#!/usr/bin/env python3
"""List the files in lib/<module>/ that do not follow the one function per file
rule: a file that defines no function of its own name, or more than one public
function. usage: tests/check-onefunc.py [-q]"""
import re, glob, os, sys

bad = {}
root = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')
for f in sorted(glob.glob(os.path.join(root, 'lib/*/*.c'))):
    base = os.path.basename(f)[:-2]
    lines = open(f, errors='ignore').read().split('\n')
    pub = []
    for i, l in enumerate(lines):
        m = re.match(r'^([A-Za-z_]\w*)\(', l)
        if m and i > 0:
            prev = lines[i - 1]
            if prev.startswith('static') or prev.startswith('#') or not re.match(r'^[A-Za-z_][\w\s\*]*$', prev):
                continue
            pub.append(m.group(1))
    names = sorted(set(pub))
    if names and (base not in names or len(names) > 1):
        bad[os.path.relpath(f, root)] = names
if '-q' not in sys.argv:
    for k, v in bad.items():
        print(k, ' '.join(v))
print('%d files deviate from one function per file' % len(bad))
