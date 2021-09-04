#!/usr/bin/env python3

import os
from compare_midi import compare
from blessed import Terminal

compiler_path = '../build/sheetc'
if os.name == 'nt':
    compiler_path = '../build/Release/sheetc.exe'

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
        midifile = f"{testfile}.mid"
        reffile = os.path.abspath(os.path.join(refdir,midifile))
        try:
            print(f"testing '{testfile}' ...", end=' ')
            os.system(f"{compiler} {infile}")
            compare(reffile, midifile)
            print(term.green + "OK" + term.normal)
        except Exception as ex:
            returnCode = 1
            print(term.red + "FAILED")
            print(term.orange + "  " + str(ex))
            print(term.normal)
        finally:
            pass #os.remove(midifile)
    exit(returnCode)

