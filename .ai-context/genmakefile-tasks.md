# genmakefile: work needed for the PIC firmware projects

Target projects: ../pictest, ../lc-meter (sources from ../libpicp, which is
their `lib/` submodule). Goal: `src/genmakefile/genmakefile.c` generates build
files that produce the same outputs and the same directory layout as the
hand-written `Makefile` + `build/*.mk` of those projects. Read
genmakefile-handoff.md first for the current state of the code.

## Facts about the existing PIC builds

- Top-level `Makefile` loops over COMPILERS, CHIPS, BUILD_TYPES, XTAL_FREQS,
  BAUD_RATES, CODE_OFFSETS and PROGRAMS and calls
  `make -f build/<compiler>.mk CHIP= XTAL= BAUD= BUILD_TYPE= PROGRAM=
  CODE_OFFSET=` for every combination.
- Layout (build/vars.mk):
  - BUILD_ID = `<type>_<mhz>mhz_<kbps>kbps[_at<codeoffset>]`
  - BUILDDIR = `bin/<compiler>-<chip>/`
  - OBJDIR   = `obj/<compiler>-<chip>/<BUILD_ID>/<PROGRAM>/`
  - outputs  = `bin/<compiler>-<chip>/<PROGRAM>_<BUILD_ID>.{hex,bin,cof,elf,cfg}`
  - `_XTAL_FREQ=48000000` gives MHZ=`intosc48` and XTAL_USED=NO_XTAL.
- Defines every build gets: `PIC<CHIP>=1 __<chip>=1 _XTAL_FREQ= UART_BAUD=`,
  debug: `__DEBUG=1 _DEBUG=1`, release: `NDEBUG=1 __NDEBUG=1`;
  include path `-I. -Ilib -Isrc`.
- Per program `<prog>_SOURCES` / `<prog>_DEFS`, partly depending on the chip
  (`ifeq $(chipl)`), e.g. pictest adds USE_HD44780_LCD/USE_SOFTSER only on some
  chips; `USE_USB=1` on the make command line adds usb sources, vpath and
  EP*_BUFFER_BASE_ADDR defines.
- lc-meter differs: main files in the project root (LC-meter.c, Cap-meter.c,
  Freq-meter.c), helpers in src/ (measure.c, print.c), XTAL instead of
  _XTAL_FREQ, BUILD_TYPE derived from `_DEBUG`, chip list per compiler,
  targets.mk is dead scaffolding overridden by vars.mk.
- xc8.mk: compiles `--pass1` from inside `obj/` (every -I becomes `-I../...`),
  link with `--codeoffset` only for a nonzero offset, `--output=-mcof,+elf`,
  runs `piccfg` on the hex to write the `.cfg`.
- sdcc.mk: `-c` per source, `-mpic14` for 16f / `-mpic16` for 18f, nonzero
  CODE_OFFSET needs a generated .lkr (sed of the gputils lkr), `--ivt-loc=
  offset+8` and an awk pass that strips hex records below the offset.
- `libpicp` is flat (`timer.c`, `timer.h`, ... plus `extra/`), not
  `lib/<name>.h` + `lib/<name>/`. `pictest/lib` and `lc-meter/lib` are empty
  here (submodule not checked out). XC8 and SDCC are not installed on this
  machine, so only generation and diffing can be checked, not compiling.

## Tasks

1. **(done, see cpp-query-skipped-branches in TODO for the gap) Queried-identifier collection in project.c.** During the lib/cpp scan
   record every identifier used in `#if`/`#elif`/`#ifdef`/`#ifndef` that is
   never defined (by the source, headers or `-D`). Two sets:
   - set A: queried with `#ifdef`, `#ifndef` or `defined()`
   - set B: used otherwise in `#if`/`#elif` (value compared)
   An identifier can be in both (USE_SER is). Needs new fields in `cpp_ctx`
   (cpp_internal.h), a new child-to-parent line type next to D/H/L, and merging
   across units. Ignore identifiers in skipped branches only where the
   preprocessor already evaluated them, and ignore compiler/system names
   (`__GNUC__`, `_WIN32`, ...). Check against `grep` counts of USE_* in
   libpicp, pictest/src and lc-meter.
2. **(done: output_switches in output.c, project_switches in project.c) Emit the queried identifiers into the build files.** Variables per set
   (`-DUSE_x=1` default off / commented), so the matrix can switch features.
   Decide the format per make type (borland, gmake, ninja, ...).
3. **(done: matrix.[ch]; per-program objects; linking of the flat lib sources is task 6, the compiler flags task 4) Build matrix.** Options for chip(s), compiler(s), build type(s), xtal
   frequencies, baud rates, code offsets. Either one file looping over the
   matrix or one file per combination (like pictest/gen-mk.sh); ask the user.
   Needs the BUILD_ID / BUILDDIR / OBJDIR naming above, MHZ computation
   (`intosc48`), `_at<offset>` suffix.
4. **(open) Compiler setup for xc8 and sdcc in compiler.c.** compiler_predefs()
   tables (XC8 v1.x/v2+/v4, SDCC pic14/pic16 with the chip macros), flag lists
   (`--chip`, `--opt`, `-G`, `--runtime`, `-mpic14/-mpic16`, `--use-non-free`,
   `-p<chip>`), object/exe extensions (.p1, .o, .hex/.cof/.cod/.elf), link
   rules, `piccfg` rule for `.cfg`, include dir rewriting for XC8 running in
   `obj/`.
5. **Code offset (bootloader) handling.** xc8 `--codeoffset`; sdcc generated
   `.lkr`, `--ivt-loc`, hex stripping. Build two variants (0x0000 and 0x2000).
6. **(partly: --library NAME makes one library of flat sources, see doc) Project discovery for the flat libpicp layout.** Programs: `src/*test.c`
   in pictest, root `*.c` in lc-meter (main() detection already exists).
   Library modules: flat `lib/*.c` with a header of the same name; no
   per-function directories. Per-program source lists come from the header
   scan (`#include` -> `lib/foo.c`) instead of `<prog>_SOURCES`. Compare the
   resulting lists with `pictest/build/vars.mk` and `lc-meter/build/vars.mk`.
7. **Chip dependent sources/defines** (e.g. no USE_HD44780_LCD on 12f1840 /
   16f628a, USE_USB adding usb sources). Find out how much of this the scan
   finds on its own (via the config-<chip>.h headers and `#if` gates) and what
   still needs explicit options.
8. **Verify.** Check out libpicp into a scratch copy of both projects (ask
   first, do not touch the real submodule dirs), generate, and diff directory
   names, object and binary names and the source/define lists against what
   `make -n` of the existing build prints for the same variables. Real
   compiles are not possible here (no XC8/SDCC); use a machine that has them
   via remote-targets if needed.
9. **Housekeeping (c-utils rules).** ChangeLog entry for every change; BUGS
   and TODO entries as described in CLAUDE.md; update doc/genmakefile.md; keep
   no stdio/stdlib/string in program code; commit only on request.

## Task 1 in detail (where the code is, nothing implemented yet)

Hook points found by reading lib/cpp and project.c:
- `lib/cpp/cpp_preprocess2.c`: `#ifdef` / `#ifndef` call
  `cpp_macro_find(pp, tok->next)`; these are the set A queries.
- `lib/cpp/cpp_read_const_expr.c`: `defined X` / `defined(X)` inside `#if` and
  `#elif` calls `cpp_macro_find(pp, tok)`; also set A.
- `lib/cpp/cpp_eval_const_expr.c`: after macro expansion, every remaining
  `TK_IDENT` is replaced by 0. These are the set B queries (identifiers used
  by value in `#if` / `#elif`). Only identifiers that were not macros at that
  point reach this loop, so "never defined" is already given for set B.
- For set A record the name only when `cpp_macro_find` returns NULL.
- Add to `struct cpp_ctx` (lib/cpp_internal.h, next to the scan_only fields):
  `strarray queried_def` (set A) and `strarray queried_val` (set B), filled
  only when `scan_only` is set; unique pushes.
- `scan_unit_inproc` in src/genmakefile/project.c: copy both arrays into the
  `unit` struct (new members), send them to the parent as new line types
  (e.g. "Q" for set A, "V" for set B) in the child/parent protocol in
  `scan_unit`, and merge into program-wide sets in `project_generate`.
- An identifier defined in some other unit or by `-D` still counts as
  "never defined" only for the unit that queried it before any definition;
  decide at merge time: drop names that appear in `project_defs`, in a
  `#define` of any scanned unit, or in `compiler_predefs()`.
- Filter out compiler / system names (`__GNUC__`, `_WIN32`, `__STDC__`,
  header guards `*_H`) so only project feature switches such as `USE_*`
  remain; start with a prefix-free rule (not defined anywhere in the scanned
  sources or predefs) and add an ignore list if it is too noisy.
- Test: run on the scratch copies under the scratchpad dir and compare the
  result with the grep census (USE_UART, USE_SOFTSER, USE_SER,
  USE_HD44780_LCD, ... in set A; USE_SER, USE_NOKIA5110_LCD, ... in set B).

## Decisions (user)

- One makefile per combination (like pictest/gen-mk.sh), not one matrix file.
- libpicp is checked out as a scratch copy, not into the real projects:
  /tmp/claude-1000/-home-roman-Sources-c-utils/a7afe223-87ae-46eb-bf37-616e3cabba1f/scratchpad/pic/{pictest,lc-meter}, each with libpicp copied into lib/.
- Queried but never defined identifiers are written to the build files as
  variables that default to off (task 2); a build switches one on from the
  command line.

## Open questions for the user

- None at the moment.

## Status 2026-10-31

Done since this file was written: tasks 1, 2, 3; flat third party libraries
(--library, --header, install rule; bzip2 and zlib build and install with
bcc55, brotli with bcc102); optional packages (HAVE_BROTLI, HAVE_LIBBZ2,
HAVE_LIBLZMA, HAVE_ZLIB found by the scan or named with --dep, switched on
with LIBBROTLI, LIBBZ2, LIBLZMA, LIBZ plus _INCLUDEDIR and _LIBDIR);
-t bcc102 (Embarcadero bcc32c, clang based, C99).

Still open: tasks 4 to 9; the skip of units that need a missing header
(unistd.h and friends); #pragma comment(lib) scanning, see
genmakefile-idea-pragma-comment-lib.md.

