# genmakefile work: handoff (2026-10-31)

Read this block first, the older text below describes the state of 10-30.

## State

- Project mode (preprocessor scan, src/genmakefile/project.c) is used for the
  compilers with a compiler_predefs() table: bcc55 and bcc102 (the clang based
  bcc32c; macros found with #pragma message probes).
- Matrix options (matrix.c), switches (USE_*), optional packages (--dep and the
  built-in table of brotli, bz2, lzma, zlib), --library NAME with --header and
  an install rule, -v/--dump logging are in; see doc/genmakefile.md.
- Borland specifics: tlib needs +"obj" (-+ to replace), -L paths with
  backslashes, bcc102 links by calling $(ILINK) (default ilink32); under wine
  the 6.90 linker of bcc102 fails with LME200, use
  `make -DILINK=/opt/bcc55/bin/ilink32.exe`. Borland make: a variable assigned in
  the file beats -DNAME=value, so overridable ones are written with !ifndef.
- Third party: third_party/{bzip2,zlib,brotli} build with --library (see
  doc/genmakefile.md); outputs inst-<compiler>/ are untracked (third_party/
  .gitignore ignores every directory). zlib needs zconf.h (copy of
  zconf.h.included); brotli needs bcc102 (C99).
- Numbers: bcc55 full build 555 error lines, 42 programs; bcc102 224, 50.

## Next steps

1. Skip units whose scan records a missing include (unistd.h ...) instead of
   failing, with a -v note (the Unix-only programs).
2. Shims for _lseek64, _isblank, _GetNativeSystemInfo (Borland), _die/_diesys.
3. #pragma comment(lib) scan (genmakefile-idea-pragma-comment-lib.md).
4. Tasks 4 to 9 of genmakefile-tasks.md (xc8/sdcc, PIC projects).
5. Optional: bcc56 as a classic target (has stdint.h and long long, no C99).
6. Commands: build the tool in build/x86_64-linux-gnu; generate with
   `genmakefile -m borland -t bcc102 -L<lib>/win32c/release ... src/`, run with
   `WINEPATH='Z:\opt\bcc102\bin' wine /opt/bcc55/bin/make.exe -i -f bcc102.mk
   -DILINK=/opt/bcc55/bin/ilink32.exe`. Generated files in the repo root
   (bcc55*.mk, bcc102*, *-Release) are untracked, delete them when done.

---- older state (10-30) ----


Temporary file, delete when the work is done. Read CLAUDE.md first (rules:
no stdio/stdlib/string in program code, one function per file in lib/,
BUGS/TODO format, ChangeLog entry for every change, commit only when asked,
no Co-Authored-By trailer).

## Goal of the user

`genmakefile -m borland -t bcc55 --create-{libs,bins} -o bcc55.mk src/`
writes a Borland makefile that builds every tool of src/ with Borland C++
5.5 (in /opt/bcc55, run as `wine /opt/bcc55/bin/make.exe -f bcc55.mk`).
Decisions the user made:
- genmakefile discovers programs (.c with a main()) itself, no per-program
  option; lib/<name>.h plus a directory lib/<name>/ means a library
  module <name>.lib/.a that programs link automatically.
- Header dependencies come from lib/cpp with the target compiler's
  predefined macros (so #if gates are evaluated), not from the text scanner.
- The old path and the new "project mode" must be unified; project mode is
  compiler agnostic. Everything specific to Borland 5.5 lives in
  src/genmakefile/compiler.c (done for the macros, extensions, libs).
- Idea only, not started (TODO entry genmakefile-compound-literals):
  write per-compiler flag lists in compiler.c as compound literals at the
  point of use instead of named tables.

## State of the repo

Pushed commits (branch main):
- 8f2e6af6 genmakefile: compiler and target setup moves into compiler.c
  (set_machine, set_chip, set_system, set_make_type, set_compiler_type,
  resolve_toolchain, set_command, compiler_types[] and the shared state
  build_tool/batchmode/cygming/comment/cross_compile/quote_args/make_*_inline
  now live in src/genmakefile/compiler.[ch]; output of the old path is
  byte identical apart from the command-line comment)
- 002a2424 genmakefile: scan the sources with the preprocessor of a known
  compiler (src/genmakefile/project.[ch], lib/cpp scan_only mode,
  compiler_predefs() in compiler.c, bcc55 .obj/.exe/.lib + cw32mt/import32,
  output.c keeps the Borland inline-file terminator "|" at column 0)

Uncommitted (mine), to commit as separate commits when the user asks:
- ChangeLog (new), CLAUDE.md (new "ChangeLog" section), doc/genmakefile.md
  (new user documentation)
- lib/typedefs.h (ssize_t for __BORLANDC__), lib/uint16.h, uint32.h,
  uint64.h (no #include <systypes.h> for Borland: it re-typedefs uint8/16/32;
  uint32/int32 typedef'd for every Borland version, Borland branch before
  _WIN32 in uint32.h)
Not mine, leave alone: CMakeLists.txt ccat hunk (tidy-cutils moved a block),
src/tcp/ssltest.c deletion, src/tcp/ctlscat.c, output.log,
lib/buffer/buffer_bz2.c. Another session "tidy-cutils" tidies the project
root and edits files under src/ and CMakeLists.txt at the same time: stage
by explicit path, split CMakeLists.txt with `git diff -U0 | filter hunks |
git apply --cached --unidiff-zero`, rerun `cmake .` in the build dir after
lib/ files appear or disappear.

## How it works now (src/genmakefile)

- main() in genmakefile.c: when no -f/-M input, positional args exist and
  compiler_predefs(tools.compiler) is non-NULL, project_generate() is used;
  otherwise the old discover_sources()/generate_all_rules() path. There is
  no --project flag any more; --lib-dir DIR (option 'Y', project_libdir,
  default "lib") and -D (also pushed to project_defs) feed the scan.
- project.c: collect() walks the args and lib/ with rdir; main_present()
  decides has_main; scan_unit() forks a child per source (lib/cpp leaks
  every token) which runs cpp with scan_only and returns lines
  "D<path>" (all headers reached), "H<path>" (project headers named by the
  unit or by non-lib headers), "L<name>" (lib modules named). Units under
  lib/<name>/ belong to module <name> when lib/<name>.h exists
  (is_module). lib_closure() adds the modules a module needs. Rules are
  created through the normal machinery: compile_rule(), lib_rule(),
  program_rule() use rule_get_sa(), set_adds(prereq), commands.compile/lib/
  link and path_output2()/path_output2_bin(); dir_rule() adds mkdir rules to
  "all". output.c prints them for any make type.
- program_rule(): objs = the program, plus foo.c for every project header
  foo.h it names when foo.c has no main(); libs = closure of the lib modules
  named by those units. Executable name = basename of the source in the bin
  dir (collisions between same-named programs are not handled).
- lib/cpp scan_only (lib/cpp_internal.h fields scan_only, inc_missing,
  inc_system): missing includes are recorded and skipped, #error skipped,
  text of #error/#warning blanked in cpp_tokenize_file (apostrophes), gcc
  host macros not defined in cpp_init_macros, headers below inc_system dirs
  (every absolute -I outside the cwd) are not read. Without this a Windows
  scan parsed windows.h for 3 s per unit.
- compiler_predefs(): bcc55 table in compiler.c (found by compiling probes
  with bcc32): __BORLANDC__=0x0551 __TURBOC__ _M_IX86=300 __WIN32__ _WIN32
  _Windows __CONSOLE__ __FLAT__ __CDECL__ __TLS__ _CPPUNWIND
  _INTEGRAL_MAX_BITS=64 _STDCALL_SUPPORTED= __MT__ _MT (for -tWC -tWM).
  Flag additions: -A adds __STDC__=1, -K adds _CHAR_UNSIGNED, -P adds
  __cplusplus __BCPLUSPLUS__ __TCPLUSPLUS__ __XDLLMT__, -tWCR adds _RTLDLL,
  -tWD adds __DLL__/_DLL (unverified). cpp32 predefines nothing by itself
  beyond that table; bcc32 has no `-dM -E` equivalent.

## Commands

Build the tool: `cd build/x86_64-linux-gnu && cmake . && cmake --build .
--target genmakefile` (the binary was briefly missing while tidy-cutils
moved debug.[ch] into lib/debug; rerun cmake if link errors mention debug_*).

Generate and run (from the repo root, stdin from /dev/null, the scan takes
about 7 s):
    build/x86_64-linux-gnu/genmakefile -m borland -t bcc55 \
        -I/opt/bcc55/include -o bcc55.mk src/ </dev/null
    WINEPATH='Z:\opt\bcc55\bin' wine /opt/bcc55/bin/make.exe -i -f bcc55.mk
Delete bcc55.mk and bcc55-Release afterwards (generated, untracked). A
full build of `all` with -i takes about 3 min 15 s. Do not give
-o an absolute path outside the cwd: the names in the file then become
../../../.. relative paths.

Compile one file by hand (fast loop):
    WINEPATH='Z:\opt\bcc55\bin' wine /opt/bcc55/bin/bcc32.exe -q -w- -c \
      -tWC -tWM -Dinline=__inline -I/opt/bcc55/include -DNDEBUG=1 \
      -n/tmp/bt lib/byte/byte_hash.c
Load tests: gdb attach does not work here, but gdb can launch the program;
`strace -f` works.

## Findings about Borland tools

- Borland make 5.2 (wine) supports !if/!elif/!else/!endif/!ifdef/!ifndef/
  !include/!error/!undef, `-DNAME=value`, `$(X:a=b)`; it does NOT expand
  nested macro names ($(CFLAGS_$(BUILD)) is empty, also via an
  intermediate). So one makefile with all four build types needs !if
  chains; not implemented. `make.exe -k` is not an option (use -i). The
  terminator of an inline file (`@&&|` ... `|`) must start in column 0.
  Directory names as targets are rebuilt when the directory is newer;
  building one object directly skips the mkdir (only `all` makes dirs).
- bcc32 needs WINEPATH to find ilink32/tlib; no bcc32.cfg exists in
  /opt/bcc55/bin, so -I/-L must be given (CPPFLAGS gets -I/opt/bcc55/include
  from the -I option, relative form ../../../../opt/bcc55/include works).
- bcc32 is C89: no declarations after statements, no for(size_t i;;) loops
  ("Improper use of typedef 'size_t'", "Undefined symbol 'i'"), no
  `inline` (compat flag -Dinline=__inline already in the bcc CPPFLAGS), no
  <stdint.h>, no <sys/time.h>, no ssize_t, no config.h (the project's
  configure header is missing for it).

## Where the bcc55 build stands

`make -i` over src/ builds 0 objects (about 1900 error lines). After the
typedefs/uintNN fixes the four sample files compile (byte_hash, buffer_put,
stralloc_catb, str_len). Remaining error classes of the last full run:
- E2209 cannot open 'config.h' (132), cannot open 'sys/time.h' (46)
- E2108 "Improper use of typedef 'size_t'" + E2451/E2379/E2378 around
  `for(size_t i = ...)` (cpp_read_string_literal 90, hashmap_fnv_hash 65,
  more in other files)
- plus whatever hides behind these; regroup with
  `grep '^Error' build.log | sed -E 's/^Error (E[0-9]+) [^ ]+ [0-9]+: /\1 /'
  | sort | uniq -c | sort -rn`

## Next steps (suggested order)

1. Commit: (a) ChangeLog + CLAUDE.md + doc/genmakefile.md, (b) the Borland
   type fixes, then push. Ask the user first.
2. Fix the remaining bcc55 compile errors in the lib/ headers and sources
   (the user's task: "fix the bcc55 compile errors"), in small steps:
   C89 declarations, config.h (HAVE_* via -D or a bcc55 section), sys/time.h
   users, then re-run the full build and regroup errors. Keep every fix
   behind __BORLANDC__ or plain C89 so other builds are unaffected; check
   the gnu/debug/musl trees still build.
3. Known bug (BUGS entry cpp-scan-stray-endif): `lib/tls/tls_get_fd.c:14`
   `#endif` at EOF without newline is reported and the unit gets no deps;
   probably append "\n" in cpp_tokenize_file.
4. Compiler agnostic: only bcc55 has compiler_predefs; add tables (or a
   `cc -dM -E` run for gcc/clang, msvc table) so the preprocessor scan
   serves the other compilers, then retire the text scanner path
   (sources.c/sourcedir.c includes_extract) or keep it for -f/-M input.
5. One makefile for all build types: emit !if "$(BUILD)" == "Debug" blocks
   for borland make (CFLAGS per type, OUT = bcc55-$(BUILD)); --debug etc.
   choose the default. Not started.
6. Per-program output names collide for same-named programs in different
   src subdirs; make the bin path mirror the source dir or warn.
7. Update ChangeLog (every change), BUGS (delete fixed entries) and
   doc/genmakefile.md (limits section) as things change.
