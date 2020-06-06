#!/usr/bin/env python3

if __name__ == '__main__':
    import sys
    if len(sys.argv[1]) < 2:
        print("missing name")
        sys.exit(1)
    name = sys.argv[1]
    nameUpper = name.upper()
    header  = ""
    cpp     = ""
    with open('_command.template.h', 'r') as f:
        header = f.read()
    with open('_command.template.cpp', 'r') as f:
        cpp = f.read()
    header = header.replace('$GUARD_NAME', f'{nameUpper}_HPP').replace('$NAME', name)
    cpp = cpp.replace('$NAME', name)
    with open(f'{name}.h', 'w') as f:
        f.write(header)
    with open(f'{name}.cpp', 'w') as f:
        f.write(cpp)
