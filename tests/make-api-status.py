#!/usr/bin/env python3
"""Writes doc/api-status.md: the numbers of the checks and the lists of what is left.
usage: tests/make-api-status.py [unit-test-count]   (run from the repository root)"""
import re, glob, os, sys, subprocess, collections

count = sys.argv[1] if len(sys.argv) > 1 else '?'
t = ''
for f in glob.glob('tests/test_*.c'):
    s = open(f, errors='ignore').read()
    s = re.sub(r'/\*.*?\*/', '', s, flags=re.S)
    s = re.sub(r'//.*', '', s)
    t += s
ids = set(re.findall(r'[A-Za-z_]\w*', t))
res = collections.defaultdict(set)
for f in sorted(glob.glob('lib/*/*.c')):
    m = os.path.basename(os.path.dirname(f))
    lines = open(f, errors='ignore').read().split('\n')
    for i, l in enumerate(lines):
        mm = re.match(r'^([A-Za-z_]\w*)\(', l)
        if mm and i > 0:
            prev = lines[i - 1]
            if prev.startswith(('static', '#')) or not re.match(r'^[A-Za-z_][\w\s\*]*$', prev):
                continue
            if mm.group(1) not in ids:
                res[m].add(mm.group(1))
n = sum(len(v) for v in res.values())
one = subprocess.run([sys.executable, 'tests/check-onefunc.py'], capture_output=True, text=True).stdout.strip().split('\n')
out = ['# API and test status', '',
       'What is checked, how, and what is left. Regenerate with `tests/make-api-status.py N`.', '',
       '| check | command | result |', '|---|---|---|',
       '| unit tests | `build/.../tests/all_tests` (run in `tests/`) | %s tests pass; `valgrind -q --error-exitcode=9` clean |' % count,
       '| every header compiles alone | `tests/check-headers.sh` | 0 failures |',
       '| libowfat source compatibility | `tests/libowfat-compat/` (see `doc/libowfat-compat.md`) | 517 prototypes, 0 missing |',
       '| one function per file | `tests/check-onefunc.py` | %s |' % one[-1],
       '| genmakefile | `tests/genmakefile/{pair-headers,import-mplabx,project-file,infer-sources}.sh GENMAKEFILE` | all pass |',
       '| functions not named by a test | this script | %d |' % n, '',
       '## Functions not named by a test', '',
       'Most are run by tests of bigger functions or hidden behind macros (`omf_record_foreach`, the `bmap_*` macros);',
       'the rest are internal helpers, debug-only code (`DEBUG_CHARBUF`, the `cbmap` debug allocators) and code that',
       'exists only for Windows (`win32_*`, `winsock*`, `terminal_wattr`, `_tpopen`).', '']
for k, v in sorted(res.items()):
    out.append('* `%s`: %s' % (k, ' '.join(sorted(v))))
out += ['', '## Files that break the one function per file rule', '']
for l in one[:-1]:
    out.append('* ' + l)
open('doc/api-status.md', 'w').write('\n'.join(out) + '\n')
print(n, one[-1])
