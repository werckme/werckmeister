#!/usr/bin/env python3
import subprocess

doc_creator            = "./scripts/doccreator/createDoc.py"
doc_creator_dirs       = ["src/compiler/commands", "examples/lua/mods", "examples/lua/voicings", "src/conductor/declarations", "src/conductor/selectors"]
autogen_references_key = '{autogen_references}'
infile_path            = "./manual.in.md"
outfile_path           = "./manual.md"

def execute(*args):
    proc = subprocess.Popen(args, stdout=subprocess.PIPE, cwd=".")
    return str(proc.stdout.read(), "utf-8") 

if __name__ == '__main__':
    autogen_references = execute(*['python3', doc_creator] + doc_creator_dirs)
    text = ''
    with open(infile_path, 'r') as f:
        text = f.read()
    text = text  \
        .replace(autogen_references_key, autogen_references)
    with open(outfile_path, 'w') as f:
        f.write(text)