#!/usr/bin/env python3
import markdown_generator as mg
from io import StringIO
import xml.etree.ElementTree as ET
import sys
heading_level = 2

class DocParser:
    def __init__(self, comment_sequence = '///'):
        self.comment_sequence = comment_sequence

    def parse(self, str:str):
        txt = lambda node: node \
            .text \
            .strip() \
            .replace('\\n\n', '$nl') \
            .replace('\n', '\n\n') \
            .replace('$nl', '\n')
        attr = lambda node, name, default="": node.attrib[name] if name in node.attrib else default
        str = str.replace(self.comment_sequence, '')
        doc_tree = ET.fromstring(f'<root>{str}</root>')
        command_node = doc_tree.find('command')
        if command_node == None:
            return
        command = CommandDto(attr(command_node, 'name'))
        command.summary = txt(command_node)
        for param in doc_tree.findall("param"):
            argument = ArgumentDto(attr(param, "name"))
            argument.summary = txt(param)
            argument.position = attr(param, 'position', "-")
            argument.range = attr(param, 'range', "-")
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
        self.range = "-"

class CommandDto:
    def __init__(self, command_name):
        self.command_name = command_name
        self.summary = ""
        self.args = []

class Printer:
    def __init__(self, dto):
        self.dto = dto


    def print(self):
        with StringIO() as stream:
            writer = mg.Writer(stream)
            writer.write_heading(f'`{self.dto.command_name}`', heading_level)
            writer.write_hrule()
            writer.writeline(self.dto.summary)
            writer.write_heading("parameters", heading_level + 1)
            table = mg.Table()
            table.add_column('name')
            table.add_column('position')
            table.add_column('description')
            table.add_column('range')
            for arg in self.dto.args:
                table.append(arg.name, arg.position, arg.summary, arg.range)
            writer.write(table)
            writer.writeline('<br>'*3)
            #finito
            stream.seek(0)
            return stream.read()
    
    def __str__(self):
        return self.print()
    
    def __repr__(self):
        return str(self)

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
    return command    

def printToc(commands):
    for command in commands:
        print(f'* [{command.command_name}](#{command.command_name})')

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
    commands.sort(key=lambda x: x.command_name)
    print('## Commands')
    printToc(commands)
    print('')
    for command in commands:
        printer = Printer(command)
        print(printer)