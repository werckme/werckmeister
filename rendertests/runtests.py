#!/usr/bin/env python3

import os
from compare_midi import compare
from blessed import Terminal
import re
import itertools
from sys import argv
compiler_path = '../build/sheetc'
if os.name == 'nt':
    if "--debug" in argv:
        compiler_path = '../build/Debug/sheetc.exe'
    else:
        compiler_path = '../build/Release/sheetc.exe'

print(f"using {compiler_path}")
def get_test_tags(sheetfile) -> list:
    lines = []
    tags = []
    with open(sheetfile, 'r') as file:
        while True:
            line = file.readline()
            if not line:
                break
            is_comment_line_or_empty = re.match('\s*--.*$', line) or len(line.strip()) == 0
            if not is_comment_line_or_empty:
                break
            tags_matched = re.findall('#([a-zA-Z0-9_()=, -]+)*', line, re.DOTALL)
            if not tags_matched:
                continue
            tags.extend(tags_matched)
    return list(set(tags))
            
def get_args_from_tags(test_tags: list) -> None:
    args_tags = [tag for tag in test_tags if tag.startswith('compiler-args')]
    args = [re.findall('compiler-args\((.*)\)', arg_tag, re.DOTALL) for arg_tag in args_tags]
    args = [arg[0].split(',') for arg in args if arg]
    args = list(itertools.chain(*args))
    args = [arg.strip() for arg in args]
    return args

if __name__ == '__main__':
    term = Terminal()
    cwd = os.path.dirname(os.path.abspath(__file__))
    refdir = os.path.join(cwd, 'references')
    indir = os.path.join(cwd, 'tests')
    isfile = lambda x: True #os.path.isfile(x)
    issheet = lambda x: isfile(x) and os.path.splitext(x)[1] == '.sheet'
    compiler = os.path.abspath(os.path.join(cwd, compiler_path))
    returnCode = 0
    if (os.path.exists(compiler) == False):
        print(f"compiler not found: {compiler}")
        exit(1)
    for testfile in [x for x in os.listdir(indir) if issheet(x)]:
        infile = os.path.abspath(os.path.join(indir,testfile))
        test_tags = get_test_tags(infile)
        midifile = f"{testfile}.mid"
        reffile = os.path.abspath(os.path.join(refdir,midifile))
        compiler_args = [infile]
        test_args = get_args_from_tags(test_tags)
        compiler_args.extend(test_args)
        try:
            print(f"testing '{testfile}' ...", end=' ')
            #print (f"{compiler} {' '.join(compiler_args)}")
            ret = os.system(f"{compiler} {' '.join(compiler_args)}")
            if ret != 0:
                raise Exception(f"compiler execution failed: {ret}")
            compare(reffile, midifile, test_tags)
            print(term.green + "OK" + term.normal)
        except Exception as ex:
            returnCode = 1
            print(term.red + "FAILED")
            print(term.orange + "  " + str(ex))
            print(term.normal)
        finally:
            pass #os.remove(midifile)
    exit(returnCode)

