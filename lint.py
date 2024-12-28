import re
import sys
from pathlib import Path

def extract_docstrings(file_path: Path) -> list[str]:

    function_signature_regex = re.compile(r'^\s*(\w+\s*)*\S+\s*(\w+::)?operator[\w\*]*\s*\(.*\)\s*(.*)\s*;\s*$')
    # class_signature_regex = re.compile(r'\s*class\s*(\w)*')

    with open(file_path, 'r') as f:
        lines = f.readlines()

    print(len(lines))

    for i, line in enumerate(lines):
        print(f"checking line {i}")
        if function_signature_regex.match(line):
            print(line)

    return


extract_docstrings(Path('Graphics/include/Windows/Keyboard.h'))
print("Finished")