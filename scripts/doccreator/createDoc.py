#!/usr/bin/env python3
import markdown_generator as mg
from io import StringIO
import xml.etree.ElementTree as ET
import sys
heading_level = 3

class CommandType:
    UNDEFINED          = 0
    INTERNAL_COMMAND   = 1
    LUA_EXTENSION      = 2

def processHeader(file_str):
    import CppHeaderParser
    cppHeader = CppHeaderParser.CppHeader(file_str)
    if len(cppHeader.classes) == 0:
        return
    class_ = list(cppHeader.classes.values())[0]
    if 'doxygen' not in class_:
        return
    comments = class_['doxygen']
    comments_parser = DocParser()
    command = comments_parser.parse(comments)
    if command != None:
        command.type = CommandType.INTERNAL_COMMAND
    return command

def processLua(file_str):
    comment_str = ""
    is_comment_line = lambda str_: str_.strip().find('--') == 0
    def get_comment_lines(f):
        line:str = f.readline()
        while line and is_comment_line(line):
            yield line
            line = f.readline()
    with open(file_str, 'r') as f:
        comment_lines = get_comment_lines(f)
        comment_str = "".join(comment_lines)
    if len(comment_str) == 0:
        return None
    comments_parser = DocParser('--')
    command = comments_parser.parse(comment_str)
    if command != None:
        command.type = CommandType.LUA_EXTENSION
        
    return command    

class DocParser:
    def __init__(self, comment_sequence = '///'):
        self.comment_sequence = comment_sequence

    def remove_comments(self, str):
        str = str.replace('\r', '')
        lines = str.split('\n')
        rm = lambda s: s.replace(self.comment_sequence, '', 1)
        lines = [rm(line) for line in lines]
        return "\n".join(lines)

    def parse(self, str:str):
        txt = lambda node: node \
            .text \
            .strip() \
            .replace('\\n\n', '$nl') \
            .replace('\n', '\n\n') \
            .replace('$nl', '\n')
        attr = lambda node, name, default="": node.attrib[name] if name in node.attrib else default
        str = self.remove_comments(str)
        doc_tree = ET.fromstring(f'<root>{str}</root>')
        command_node = doc_tree.find('command')
        if command_node == None:
            return
        command = CommandDto(attr(command_node, 'name'))
        command.include = attr(command_node, 'using')
        command.summary = txt(command_node)
        for param in doc_tree.findall("param"):
            argument = ArgumentDto(attr(param, "name"))
            argument.summary = txt(param)
            argument.position = attr(param, 'position', "-")
            argument.type = attr(param, 'type', "-")
            try:
                posVal = int(argument.position) + 1
                argument.position = posVal
            except: pass
            command.args.append(argument)
        return command

class ArgumentDto:
    def __init__(self, arg_name):
        self.name = arg_name
        self.summary = ""
        self.position = "-"
        self.type = "-"

class CommandDto:
    def __init__(self, command_name):
        self.command_name = command_name
        self.summary = ""
        self.type = CommandType.UNDEFINED
        self.include = None 
        self.args = []

class Printer:
    def __init__(self, dto):
        self.dto = dto
        self.heading_level = heading_level
    def print_include(self, writer:mg.Writer):
        if self.dto.include == None:
            return
        if self.dto.type != CommandType.LUA_EXTENSION:
            return
        writer.write_heading("include extension", self.heading_level + 1)
        writer.writeline(f'`using "{self.dto.include}";`')
        writer.writeline('')

    def print_parameters(self, writer):
        writer.write_heading("parameters", self.heading_level + 1)
        if len(self.dto.args) == 0:
            writer.writeline('*no parameters*')
            return
        table = mg.Table()
        table.add_column('name')
        table.add_column('position')
        table.add_column('description')
        table.add_column('type')
        for arg in self.dto.args:
            table.append(arg.name, arg.position, arg.summary, arg.type)
        writer.write(table)


    def print(self):
        with StringIO() as stream:
            writer = mg.Writer(stream)
            writer.write_heading(f'`{self.dto.command_name}`', self.heading_level)
            writer.writeline(self.dto.summary)
            self.print_parameters(writer)
            self.print_include(writer)
            writer.writeline('<br>'*3)
            #finito
            stream.seek(0)
            return stream.read()
    
    def __str__(self):
        return self.print()
    
    def __repr__(self):
        return str(self)

def printToc(title, commands):
    print(f'## {title}')
    for command in commands:
        print(f'* [{command.command_name}](#{command.command_name})')  

def printCommands(commands):
    internal = [cmd for cmd in commands if cmd.type == CommandType.INTERNAL_COMMAND]
    internal.sort(key=lambda x: x.command_name)
    lua_ext  = [cmd for cmd in commands if cmd.type == CommandType.LUA_EXTENSION]
    lua_ext.sort(key=lambda x: x.command_name)
    printToc('Commands', internal)
    print('')
    printToc('Lua Extensions', lua_ext)
    print('')
    print('## Commands')
    for command in internal:
        printer = Printer(command)
        print(printer)
    print('')
    print('## Lua Extensions')
    for command in lua_ext:
        printer = Printer(command)
        print(printer)        




file_handler = {'.h': processHeader, '.lua': processLua}

if __name__ == '__main__':
    import argparse
    from os import listdir
    from os.path import isfile, splitext, join
    parser = argparse.ArgumentParser(description='generates markdown from argumentNames.h')
    parser.add_argument('input', help='the input file', nargs='+')
    args = parser.parse_args()
    is_ignore = lambda file_path: file_path[0] == '_'  
    is_header = lambda file_path: splitext(file_path)[-1] == '.h'
    is_lua    = lambda file_path: splitext(file_path)[-1] == '.lua'
    in_dirs = args.input
    files = []
    for in_dir in in_dirs:
        files.extend([join(in_dir, file) for file in listdir(in_dir) if not is_ignore(file)])
    files = [file for file in files if isfile(file) and (is_header(file) or is_lua(file))]
    commands = []
    for file in files:
        ext = splitext(file)[1]
        command = file_handler[ext](file)
        commands.append(command)
    commands = [command for command in commands if command != None]
    printCommands(commands)
