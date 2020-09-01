import sys
from os import path
import json

base_dir = ""
json_dict = []

def process_file(file):
    rel = path.relpath(file, base_dir)
    content = ""
    with open(file, "r") as f:
        content = f.read()
    obj = {
        "path": rel,
        "data": content
    }
    json_dict.append(obj)

for arg in sys.argv[1:]:
    if path.isdir(arg):
        base_dir = arg
    if path.isfile(arg):
        process_file(arg)

json_str = json.dumps(json_dict) 
with open("werckmeister-auxiliaries.json", "w") as f:
    f.write(json_str)
