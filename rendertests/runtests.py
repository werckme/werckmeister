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

TAG_SKIP_TEST = 'ignore'
TAG_POST_TEST = 'post-test'
print(f"using {compiler_path}")
def get_test_tags(sheetfile) -> list:
    lines = []
    tags = []
    with open(sheetfile, 'r') as file:
        while True:
            line = file.readline()
            if not line:
                break
            is_comment_line_or_empty = re.match(r'\s*--.*$', line) or len(line.strip()) == 0
            if not is_comment_line_or_empty:
                break
            tags_matched = re.findall('#([a-zA-Z0-9_()=, -]+)*', line, re.DOTALL)
            if not tags_matched:
                continue
            tags.extend(tags_matched)
    return list(set(tags))
            
def get_args_from_tags(test_tags: list) -> None:
    args_tags = [tag for tag in test_tags if tag.startswith('compiler-args')]
    args = [re.findall(r'compiler-args\((.*)\)', arg_tag, re.DOTALL) for arg_tag in args_tags]
    args = [arg[0].split(',') for arg in args if arg]
    args = list(itertools.chain(*args))
    args = [arg.strip() for arg in args]
    return args

def execute_postprocess_script(name, testdata: dict):
    from importlib.machinery import SourceFileLoader
    lib = SourceFileLoader(name, f"./postprocess_scripts/{name}.py").load_module()
    lib.execute(testdata)

def postprocess(testdata: dict, test_tags: list[str]) -> None:
    extract_file = re.compile(r"post-test\((.+?)\)")
    try:
        post_scripts = [extract_file.match(x)[1] for x in test_tags if x.strip().startswith(TAG_POST_TEST)]
    except:
        raise RuntimeError("failed to extract script file for postprocess")
    for post_script in post_scripts:
        print(f"\n\t post process: {post_script} ", end= '')
        execute_postprocess_script(post_script, testdata)

if __name__ == '__main__':
    term = Terminal()
    cwd = os.path.dirname(os.path.abspath(__file__))
    refdir = os.path.join(cwd, 'references')
    indir = os.path.join(cwd, 'tests')
    isfile = lambda x: True #os.path.isfile(x)
    issheet = lambda x: isfile(x) and os.path.splitext(x)[1] == '.sheet'
    compiler = os.path.abspath(os.path.join(cwd, compiler_path))
    returnCode = 0
    ranAndFailedAndIgnored = [0, 0, 0]
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
            ranAndFailedAndIgnored[0] += 1
            print(f"testing '{testfile}' ...", end=' ')
            if TAG_SKIP_TEST in test_tags:
                ranAndFailedAndIgnored[2] += 1
                print(term.orange + "IGNORED VIA TAG" + term.normal)
                continue
            ret = os.system(f"{compiler} {' '.join(compiler_args)}")
            if ret != 0:
                raise Exception(f"compiler execution failed: {ret}")
            compare(reffile, midifile, test_tags)
            postprocess({"referenceMidiFile": reffile, "midiFile": midifile, "sheetFile": infile}, test_tags)
            print(term.green + "OK" + term.normal)
        except Exception as ex:
            ranAndFailedAndIgnored[1] += 1
            returnCode = 1
            print(term.red + "FAILED")
            print(term.orange + "  " + str(ex))
            print(term.normal)
        finally:
            pass #os.remove(midifile)
    print("-----------------------------------")
    print(f"Tested Executable: {compiler}" )
    print(f"Total={ranAndFailedAndIgnored[0]}\tFailed={ranAndFailedAndIgnored[1]}\tIgnored={ranAndFailedAndIgnored[2]}")
    exit(returnCode)
