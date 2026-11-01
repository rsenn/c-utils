# genmakefile: specification of the complete tool

Status: **work in progress -- only section 1 (the reassessment) is written;
sections 2 to 15 below are the planned outline and still to be written**
(tracked in `TODO` as genmakefile-full-spec). Target specification, started
2026-11-01. It describes the program
that the plans in `.ai-context/` point at, not the program that exists today.
Statements about the current program are marked *(today)*; everything else is
new. Section 1 reassesses the four planning documents item by item and says
what happened to each; sections 2 to 14 will be the specification; section 15
the path from the current code to it.

Source documents, all in `.ai-context/`:

| short name | file | what it is |
| --- | --- | --- |
| HANDOFF | `genmakefile-handoff.md` | state of the Borland work and the next steps |
| TASKS | `genmakefile-tasks.md` | tasks 1 to 9 for the PIC firmware projects |
| PRAGMA | `genmakefile-idea-pragma-comment-lib.md` | library names from `#pragma comment(lib)` |
| BCC55 | `bcc55-errors.md` | the compile errors of Borland C++ 5.5, classified A to K |

---

## 1. Reassessment of the planning documents

The question asked of every item: *does it belong in genmakefile, is the
design still the right one, and what is the smallest general form of it?*
Verdicts: **done** (nothing left), **keep** (as planned), **revise** (the goal
stands, the design changes), **move** (belongs elsewhere), **drop**.

### 1.1 The decisions of the user (HANDOFF "Goal of the user")

| decision | verdict | comment |
| --- | --- | --- |
| genmakefile finds programs itself (a `.c` with `main()`), no per-program option | **keep**, extend | The rule is right for the layouts this project has. For layouts where it is ambiguous (several `main()` in one file under `#if`, test files, example files, generated mains) there must be a way to say so without per-program options: a project file (section 11.2) with *roles* for directories, not entries for programs. |
| `lib/<name>.h` + `lib/<name>/` is a module, `<name>.lib/.a`, linked automatically | **keep**, generalize | This is one *layout* of several (section 4). The detection and the closure rule (a module links the modules its units name) are kept as they are. |
| header dependencies from `lib/cpp` with the target's predefined macros, not from the text scanner | **keep**, finish | Correct, and the reason genmakefile is better than a regex scanner. Today it exists for two compilers only. Section 5 makes it available for all of them, by probing instead of tables where the compiler can be asked. |
| unify the old path and "project mode"; project mode is compiler agnostic | **keep**, this is the main refactor | Two pipelines is the source of most of the open bugs (BUGS: `genmakefile-srcdir-lib-rule-dead-dep-check`, `-shell-mode-*`). One pipeline: discovery, analysis, graph, emit (section 3). The old path survives only as an *input adapter* (`-f` build log, `-M` CMake directory). |
| everything specific to Borland 5.5 lives in `compiler.c` | **revise** | Right idea, wrong container. 1,206 lines of C that must be recompiled for every new compiler flag is the thing that makes "support every compiler" expensive. Compiler knowledge becomes *data*: descriptors (section 6). `compiler.c` shrinks to the interpreter of descriptors. |
| compound literals instead of named tables (TODO `genmakefile-compound-literals`) | **revise** | The instinct (keep data next to its use, no named tables) is the instinct of data-driven descriptors. A compound literal is still code. The descriptors in section 6 give the same locality without recompiling, and a built-in copy is generated from them into the binary so that the tool stays a single file. |

### 1.2 Next steps of HANDOFF (the 10-31 block and the older list)

| # | item | verdict | disposition |
| --- | --- | --- | --- |
| 1 | skip units whose scan records a missing include (unistd.h ...) | **revise** | BCC55 itself warns this "hides real problems of portable files". The right form is a *requirement* system: the scan classifies each missing include (section 5.6) as *platform header of another system* (unistd.h on Windows), *optional package* (openssl) or *unknown*. Units that need a capability the target lacks are left out **and reported**; unknown ones are errors. `--missing=skip\|warn\|error` overrides. Also covers BCC55 B, F, G, K and its two "optional" items. |
| 2 | shims for `_lseek64`, `_isblank`, `_GetNativeSystemInfo`, `_die/_diesys` | **move** | These are missing symbols of a C library, i.e. `lib/windoze.h` and friends. genmakefile's part is only to *tell*: a link-time undefined-symbol report (section 5.9). |
| 3 | `#pragma comment(lib)` scan | **keep**, generalize | PRAGMA is right and its finding that bcc55 has no such pragma kills the narrow form. Generalized to *library hints* (section 5.8): from pragmas, from a header-to-library table (`<shlwapi.h>` implies `shlwapi.lib`), and from the project file, applied by the capability of the compiler. The header table works for bcc55, which has no pragma. |
| 4 | tasks 4 to 9 (xc8, sdcc, PIC projects) | **keep**, restructure | Becomes the firmware section (section 9), built on descriptors so that xc8, sdcc, xc16, xc32, avr-gcc, arm-none-eabi-gcc, z88dk, cc65 and stm8 sdcc are all the same kind of thing. |
| 5 | optional bcc56 as a classic target | **keep**, free | In the descriptor model it is a ten line variant of `bcc55`. |
| 6 | commands (how to run, wine variables) | **move** | User documentation (`doc/genmakefile.md`), already there. The wine facts become backend/toolchain *notes* in the descriptors so `genmakefile --doctor` can check them (section 12). |
| old 1-2 | commit, fix compile errors of bcc55 | **move** | Source work; see 1.5. |
| old 3 | `cpp-scan-stray-endif` | **keep** | A bug in `lib/cpp` (`cpp_tokenize_file` needs a newline at EOF). First thing to fix: it silently drops dependencies. |
| old 4 | only bcc55 has `compiler_predefs`; add tables or a `cc -dM -E` run, then retire the text scanner | **keep**, sharpen | Probe first, table second, text scanner last (section 5.2). The text scanner is *kept*, as a fast fallback and for `-f`/`-M`, not retired: it needs no compiler and genmakefile must work on a machine with none (that is the point of generating files for another machine). |
| old 5 | one makefile for all build types with `!if "$(BUILD)" == ...` | **revise** | Not a Borland feature to build the model around. Capability-driven (section 7.3): each backend states whether a variable can select a flag set; if yes, one file holds all build types, if no the file holds one and `--all-types` writes one file per type. |
| old 6 | same-named programs collide | **keep** | Naming policy (section 8.4): `flat` (today), `mirror`, `prefixed`; a collision under `flat` is an error that names both sources and the fix, never a silent overwrite. |
| old 7 | keep ChangeLog, BUGS, docs current | **drop** as a task | Standing rule of the repository (`CLAUDE.md`). |

### 1.3 Findings about Borland tools (HANDOFF)

Every one of them becomes a *capability* or a *quirk* in the Borland make
backend and the bcc descriptors, so that nothing about Borland lives in code:

* no nested macro expansion (`$(CFLAGS_$(BUILD))`): capability `nested-macro: no`.
* `!if/!elif/!else/!ifdef/!ifndef/!include/!error/!undef`, `-DNAME=value`, `$(X:a=b)`
  work; a variable assigned in the file beats `-DNAME=value`, so overridable
  variables use `!ifndef` (today); `-k` is not available, `-i` is.
* the terminator of an inline file (`@&&|` ... `|`) is in column 0; `tlib`
  needs `+"obj"` and `-+` to replace.
* directories as targets are rebuilt when the directory is newer, and building
  a single object skips `mkdir`: capability `mkdir-as-dependency: unreliable`,
  so the backend writes `if not exist` commands into the object rule instead
  of relying on an `all` prerequisite. (Today only `all` makes directories.)
* bcc32 is C89: no declarations after statements, `for(size_t i...)`, `inline`,
  `<stdint.h>`, `ssize_t`: toolchain property `c-standard: c89`; the scan can
  warn about C99 constructs? **No**: that is a compiler's job. It is a property
  used for `--lang-c` flag selection and for the report of section 5.9.

### 1.4 TASKS

| task | verdict | comment |
| --- | --- | --- |
| 1 queried identifiers (sets A and B) | **done** | `project_switches`. Revision: the heuristic "USE_ prefix and never given a value" is good; keep it as default, add `--switch-pattern` (a glob) instead of only `--switch-prefix`, and let the project file list names explicitly. See also TODO `cpp-query-skipped-branches`. |
| 2 emit switches into the build files | **done** | `output_switches`. Per-backend syntax is a capability (section 7.3). |
| 3 build matrix | **done**, extend | `matrix.[ch]`. One file per point is the user's decision (kept). Added: an optional *driver* that reproduces the loops of the hand written top-level `Makefile` of pictest (section 8.2), so the files for all points can be built with one command. |
| 4 compiler setup for xc8, sdcc | **keep**, as descriptors | The facts in TASKS ("compiles `--pass1` from inside `obj/`, every -I becomes `-I../`", `--output=-mcof,+elf`, `piccfg` after link, `-mpic14/-mpic16`) become descriptor *rules* (section 6.4); they are exactly the kind of thing that does not fit flag lists. |
| 5 code offset (bootloader) | **keep** | Needs *generated artifacts* (an `.lkr` made from the gputils one by a transformation, a hex filter). New rule kind: `generate` (section 6.5). |
| 6 project discovery for flat libpicp | **keep**, partly done | `--library NAME` is the first step. The general form is the *flat* layout (section 4.4): the sources of a directory with a header of the same name are a module without a subdirectory. |
| 7 chip dependent sources and defines | **keep** | The scan evaluates `#if` per chip because the chip macros are in the predefs of the chip; what remains explicit are *source additions by switch* (`USE_USB` adding usb sources). Spec: `[switch.USE_USB] sources = ...` in the project file (section 11.2). |
| 8 verify against the real projects | **keep**, as conformance test | Section 14.4: the PIC projects become fixtures with expected outputs (names, source lists, defines). |
| 9 housekeeping | **drop** | Standing rule. |

Decisions of the user in TASKS (one makefile per combination; scratch copies
of libpicp; queried identifiers default off) are all kept.

### 1.5 BCC55: where each class is fixed

A genmakefile spec cannot fix C89 declarations, but it can make the *rest*
automatic. The classification A to K splits as follows:

| class | what | fixed in |
| --- | --- | --- |
| A C99 in the code | source | library (mechanical); not genmakefile |
| B POSIX headers Borland lacks | `sys/time.h`, `unistd.h`... | `lib/cpp.h` for the one include that is wrong; for the Unix-only units genmakefile's **requirement system** (5.6, 6.6) |
| C `alloca`, config.h | `HAVE_*` | source for `alloca`; genmakefile **probe** feature (9.4) replaces the missing config header without a configure step |
| D C99 types | `uint32_t`, `intptr_t` | `lib/typedefs.h` |
| E missing constants and APIs | winsock/shell | source; genmakefile reports them (5.9) |
| F `#warning` | iom | source or requirement system |
| G module without its library | tls without OpenSSL | requirement system: the unit needs capability `openssl`, which is a *package* (9.3) |
| H DRIVE() macro bug | real bug on every Windows compiler | source (**fixed separately**) |
| I label undefined | probably consequence of A | recheck after A |
| J ftpcopy unfinished | not buildable anywhere | project file `skip` list (11.2) |
| K Unix-only `sig_dfl` | `struct sigaction` | requirement system |

The documents' two "optional" ideas (leave out a unit with a missing include;
leave out a program whose libraries have no object) are one idea, and it is
**kept** in the form of 5.6: omission is a *reported decision* with a reason,
never a silent default.

### 1.6 PRAGMA

Kept, with the correction that makes it general (see 1.2 #3). Two facts it
records are normative in this spec: bcc32c (bcc102) honors
`#pragma comment(lib, ...)` like MSVC; bcc55 and bcc56 do not; mingw ignores
it. The consequence is a per-compiler property `pragma-comment-lib: yes|no`.

### 1.7 What the documents leave out

Things a complete tool needs that none of the four mentions; each has a
section below:

* incremental correctness: header dependencies at *build* time (`-MMD`
  depfiles), not only the static ones from the scan (7.5);
* the generated file keeping itself current (regeneration rule, 11.6);
* install, uninstall, dist, clean and the test target (10);
* out-of-tree builds and every placement of objects (4.7);
* shared libraries and loadable modules for more than gcc (TODO
  `genmakefile-create-module-gcc-only`);
* a configuration probe so that `config.h` does not have to exist (9.4);
* pkg-config integration (the project has `pkgcfg`) (9.3);
* `compile_commands.json` and other machine readable outputs (11.4);
* a cache for the scan: it takes 7 s on the whole tree under bcc55 (13);
* determinism of the output (11.5), which the BUGS file shows was a source of
  real defects (hash order);
* diagnostics policy (TODO `genmakefile-scan-diagnostics`: hundreds of lines
  of preprocessor noise hide `-v`).


---

## 2. Goals, non-goals, principles

### 2.1 What "usable in production" means

The author's use cases, in the order they must work:

1. **PIC firmware** (pictest, lc-meter, libpicp): XC8 and SDCC, many chips,
   crystal/baud/bootloader-offset matrix, `.hex/.cof/.elf/.cfg` outputs, the
   exact directory layout of the hand written `Makefile` + `build/*.mk`.
2. **This repository** (c-utils): about 100 tools over 90 modules, built with
   gcc/clang (gmake, ninja), MSVC (nmake), Borland 5.5/10.2 under wine, tcc,
   dietlibc/musl static, Android NDK.
3. **Third party trees** dropped in as-is (zlib, bzip2, brotli): a flat
   library with an install rule.
4. **Porting**: point it at a foreign tree and learn what it needs.

A tool is *production grade* when, for these, all of the following hold:

| property | test |
| --- | --- |
| **correct** | the generated build produces the same artifacts as the reference build (names, flags, defines, sources) -- section 14.4 |
| **incremental** | touching one header rebuilds exactly the objects that include it, under every backend |
| **deterministic** | same inputs give the same bytes; no hash-order, no timestamps, no absolute host paths unless asked |
| **self-diagnosing** | every omission, guess and fallback is reported with a reason; nothing is silently dropped |
| **fast** | a no-change regeneration of this repository takes under 1 s (cache, 13.2) |
| **single file** | one static binary, no runtime data files required (descriptors are compiled in, overridable) |
| **tested** | a conformance suite runs without any vendor compiler (14) |

### 2.2 Non-goals

* Not a build *system* with its own executor. It emits files for existing
  ones. Running them is the job of make/ninja/MPLAB.
* Not a package manager. It consumes `pkg-config`, environment variables and
  explicit paths.
* Not a configure script. It probes (9.4) to replace `config.h`, but does not
  attempt autoconf's breadth.
* Does not fix the sources. Where a source cannot build for a target it says
  so (5.6, 5.9).

### 2.3 Principles

1. **Describe, do not script.** What a compiler/make/layout is lives in
   *data*; code interprets data.
2. **Ask the tool first.** Predefined macros, include paths, versions: run the
   compiler when it is available; fall back to tables for compilers that
   cannot be asked (bcc32, xc8 without license) or hosts that lack them.
3. **Generate for another machine.** The host that runs genmakefile need not
   have the compiler, make or the target OS.
4. **One pipeline.** Discovery, analysis, graph, emission. Input adapters
   (build log, CMake dir) feed the same graph.
5. **Capability, not type switches.** Backends and toolchains declare what
   they can do; the generator picks a strategy from the declaration.
6. **Loud omissions.** A unit, program or library that is left out gets a
   line in the report with a reason and the option that would force it.
7. **Match the house style.** One function per file, no stdio/stdlib/string
   in program code (CLAUDE.md) -- the tool is built from `lib/` modules.

---

## 3. Architecture

```
 inputs                 pipeline                                 outputs
 ------                 --------                                 -------
 directories  ─┐                                              ┌─ gmake / make
 project file ─┤  1 discover ─ 2 analyse ─ 3 resolve ─ 4 graph ┤─ ninja
 build log  ───┤  (layout)   (cpp scan,   (toolchain,  (IR)    ├─ nmake / borland
 CMake dir  ───┘              probes)      matrix)             ├─ shell / batch
                                                                ├─ MPLAB X / cmake
                                         cache (13.2) ─────────└─ compile_commands.json
```

### 3.1 Stages

1. **Discover** (section 4): walk the roots, classify files by *role*
   (program source, library source, header, generated, ignored), group them
   into modules and programs according to the layout in effect.
2. **Analyse** (section 5): per translation unit, run the preprocessor with
   the target's predefs and flags; record headers reached, modules named,
   queried switches, library hints, missing includes, `main()` presence.
   Results are cached by content hash.
3. **Resolve** (sections 6, 8): select the toolchain descriptor, expand the
   build matrix, bind switches/packages/chip data to a *build point*.
4. **Graph** (section 7.1): build the intermediate representation: typed
   nodes (source, object, archive, program, generated file, phony) with
   typed edges and attached command templates.
5. **Emit** (section 7): a backend writes the graph in its syntax, using the
   capability table to choose strategies.

*(today)*: stages 1+2 are `collect()`/`scan_unit()` in `project.c` for
`bcc55`/`bcc102` and `discover_sources()` in `sourcedir.c` for the rest;
stage 3 is `compiler.c`; stages 4+5 are `rule.c`/`output.c`. Unification
means stages 1 and 2 serve every compiler.

### 3.2 The intermediate representation

Everything the backends need, nothing make-specific:

```
node   := id, kind, path, role
kind   := source | header | object | archive | program | module | generated
          | phony | directory
edge   := target -> prerequisite  (kind: build | order-only | implicit)
action := template name + variable bindings   (e.g. compile, archive, link)
point  := toolchain, chip?, build-type, switches, packages, matrix coords
```

Variables (`CC`, `CFLAGS`, `CPPFLAGS`, `LDFLAGS`, `LIBS`, plus per-point
ones) are first-class in the IR with a *default* and an *override policy*
(`overridable`, `fixed`, `append`), so the backend decides how to spell it
(`CFLAGS ?= ...`, `!ifndef CFLAGS`, `set CFLAGS=...`, ninja `var =`).

### 3.3 Determinism contract

* All collections in the IR are ordered by (role, path) or by insertion
  order of the walk, which is sorted. No iteration over hash order.
  (`set` is insertion ordered since 2026-11; this is why.)
* Paths are written relative to the output file unless `--absolute`.
* No timestamps, user names, host names in output, except one comment line
  with the command line (`--no-header` omits it).

---

## 4. Layouts: the directory configurations

A *layout* is a rule set mapping directories to roles. The tool has a
catalog; `--layout NAME` or the project file selects, otherwise it is
detected (4.8). Layouts compose: one tree may have a module library under
`lib/`, programs under `src/` and tests under `tests/`.

### 4.1 Roles

| role | meaning | linked into |
| --- | --- | --- |
| `program` | source with `main()` (or listed) | an executable named after it |
| `module` | sources forming a named library | `<name>.lib/.a` |
| `shared` | sources of a dynamic library / loadable module | `.dll/.so` |
| `test` | like `program` but in the `test` target, not `all` | executable, run by `check` |
| `example` | like `program`, target `examples` | executable |
| `generated` | produced by a rule (lex/yacc/embed/lkr) | consumed by others |
| `ignore` | never looked at | nothing |
| `vendor` | third party, scanned for headers only | by `--library` |

### 4.2 Layout catalog

| name | shape | notes |
| --- | --- | --- |
| `modular` *(today: c-utils)* | `lib/<m>.h` + `lib/<m>/*.c` = module `m`; `src/**.c` programs | one function per file; closure rule: module needs modules its units name |
| `flat-lib` *(today: `--library`, libpicp)* | `lib/*.c` each with `lib/<n>.h`; no subdirs | each header+source pair is a *unit*; a program links the units its includes name, not the whole directory |
| `flat-single` | one directory of `.c`/`.h` | each `.c` with `main()` a program; rest linked per includes (`foo.h` implies `foo.c`) |
| `src-include` | `src/*.c` and `include/*.h` (GNU style) | headers public, sources private |
| `per-dir` | each subdirectory is a program or library (has a build marker or a `main`) | `app/`, `libfoo/` |
| `monorepo` | several of the above under `packages/*/` | each package a project; shared `third_party/` |
| `autotools-like` | `configure.ac`, `Makefile.am` | *input adapter only*: read `SOURCES` lists (`_SOURCES` variables), not run autotools |
| `cmake-tree` | `CMakeLists.txt` | adapter: `-M` reads a configured build directory (today) |
| `vendor-tree` | a dropped-in third party tree | `--library NAME --header H`, optional `--exclude` glob |
| `firmware` | `main` files at the root, helpers in `src/`, a vendor `lib/` submodule (lc-meter) or `src/*test.c` mains (pictest) | firmware section 9 |
| `kernel-like` | `drivers/x/y.c` with `obj-$(CONFIG_X) += y.o` | out of scope; read-only adapter possible later |

### 4.3 Program discovery

Order of rules (first match wins, each reported with `-v`):

1. project file `program` entries and `role` globs (11.2);
2. a source with a `main` per the scan (`main(`, `WinMain(`, `wmain(`,
   `app_main`, plus the `--entry` names of a toolchain, e.g. `void main(void)`
   of XC8);
3. a source whose name matches `program-glob` (`*test.c` in pictest);
4. none: the directory is a library candidate.

A file with a `main` under `#if` is a program only when the `#if` is true
for the point (the scan evaluates it). Two `main()`s in one file under
different `#if`s are a program per point.

### 4.4 Flat libraries

For the layout `flat-lib` the module unit is a **header+source pair**:

* unit `timer` = `lib/timer.c` (+ `lib/timer.h`);
* a program that includes `timer.h` (directly or through another header
  that the scan reaches) links `timer.o` and then, transitively, the units
  `timer.c` names;
* directories under it (`extra/`) are the same layout recursively, with the
  directory name not part of the module name unless it collides;
* sources with no header of their own (`main.c`, `isr.c`) are *attached*
  to the program that names them via the project file or are programs.

This is what replaces the hand written `<prog>_SOURCES` lists of pictest.

### 4.5 Naming collisions

Two programs `foo` under different directories:
`--names=flat` (today) is an error that names both paths; `mirror` writes
`bin/<dir>/foo`; `prefixed` writes `bin/<dir>-foo`. Two modules with the
same name are always an error. Two objects with the same relative path under
different roots cannot occur because objects mirror the source path.

### 4.6 Object placement

| `--objects=` | path of `a/b.c`'s object | when |
| --- | --- | --- |
| `mirror` (default) | `<builddir>/a/b.<ext>` | out-of-tree, any depth |
| `flat` | `<builddir>/b.<ext>` | collisions are errors |
| `per-program` *(PIC)* | `obj/<toolchain>-<chip>/<build id>/<program>/b.<ext>` | a program may get other macros than the next one |
| `in-source` | `a/b.<ext>` | classic make style |

### 4.7 Out-of-tree and in-tree

The output file may be anywhere; paths in it are made relative to it (never
`../../../..` explosions: if the relative path leaves more than 3 levels the
tool switches to a `SRCROOT` variable and says so). `VPATH`/`vpath` is used
only by backends that have it and only when `--vpath` is given; the default is
explicit paths. `-w/--workdir` stays for compatibility.

### 4.8 Detection

The tool inspects the roots once (cheap, names only) and chooses:

```
lib/<m>.h and lib/<m>/            → modular
lib/*.c with same-named .h, no dirs → flat-lib
*.ini / mplab-nbproject / chip given → firmware (adds to the above)
Makefile.am                       → autotools-like
CMakeLists.txt                    → cmake-tree (needs -M or --cmake-configure)
```

`--layout=?` prints the detection with the evidence; `-v` prints it always.

---

## 5. Analysis

### 5.1 The translation-unit scan

For each unit the child process runs `lib/cpp` with `scan_only`
(tokenizer does not leak, results come back as lines). It returns:

| line | meaning |
| --- | --- |
| `D<path>` | every header reached (dependency) |
| `H<path>` | project headers named by the unit |
| `L<name>` | lib module named |
| `Q<name>` | identifier queried with `#ifdef`/`defined` and never defined (switch, set A) |
| `V<name>` | identifier used by value in `#if` and never defined (switch, set B) |
| `P<name>` | library hint from a pragma (5.8) |
| `M<path>` | include that could not be found (5.6) |
| `E<text>` | diagnostics, collected, shown by `-v -v` (5.10) |
| `X`       | the unit defines `main` for this point |

Reading the line protocol is the only coupling between scan and generator,
so the scan can be moved to threads or a server later.

### 5.2 Predefined macros: probe first

For a toolchain whose descriptor says `predef: probe`, run once per point
(cached on the compiler's identity: path + mtime + flags):

| compiler family | probe |
| --- | --- |
| gcc, clang, tcc, icc, xc16/xc32, avr-gcc, arm-none-eabi-gcc | `cc -dM -E -x c /dev/null` plus flags |
| msvc, clang-cl | `cl /Zc:preprocessor /E` on a generated file printing `_MSC_VER` etc.; fallback table per `cl` version banner |
| bcc32c / bcc102 | `#pragma message` probe (today) |
| bcc55 | table (today); no way to ask |
| xc8 | `xc8-cc -dM -E` for v2+, table (chip macros) for v1 |
| sdcc | `sdcc -E -dM`; plus `-m<arch>` and `-p<chip>` set `__SDCC_*` |

Tables are also *descriptors* (6.2). A probe result is stored beside the
cache and reported by `--doctor` (12.3).

### 5.3 System include directories

`#include <stdint.h>` must resolve to the toolchain's, not the host's. Source
of truth, in order: `cc -E -Wp,-v -x c /dev/null` output, descriptor
`sysinclude`, `-I` options. Directories absolute and outside the project are
*system*: headers found there are not read for dependencies (today's 3 s per
unit win with `windows.h`), but their **existence** answers `has-include`.

### 5.4 Switches (queried identifiers)

Kept as designed (TASKS 1-2): sets A and B. Refinements:

* match by glob list (`--switch-pattern 'USE_*,ENABLE_*'`), default `USE_*`;
* the project file may name switches and give each a *kind*:
  `flag` (`-DUSE_X=1` or nothing), `value` (`-DBAUD=9600`), `choice`
  (`one of a b c`), `source` (adds files, 4.4/11.2);
* defaults come from (a) the project file, (b) the value the scan finds in
  `#ifndef X / #define X v` guards, (c) off;
* the make file exposes `USE_X` as an overridable variable
  (`make USE_SER=1`); the matrix can range over them (8.1).

### 5.5 Conditional compilation per chip

Chip macros come from the toolchain descriptor (`PIC16F1829=1`, `__16f1829`,
`_16F1829`, ...) so the same scan that selects `#if defined(_16F1829)`
branches yields chip-specific dependencies. Where a vendor includes
`<pic16f1829.h>` through `<xc.h>`, the chip header is *vendor* (system) and
its `#define`s are read from the descriptor's chip table instead of the
header file, which does not exist on the host.

### 5.6 Requirements and omissions

Each unit gets a set of **requirements** from its scan:

| source of the requirement | example | classification |
| --- | --- | --- |
| missing system include | `unistd.h`, `sys/socket.h` | `platform:posix` |
| | `windows.h`, `winsock2.h` | `platform:win32` |
| | `openssl/ssl.h` | `package:openssl` |
| | `xc.h` for a non-PIC target | `toolchain:xc8` |
| `#error` reached in the unit | `#error "unsupported"` | `unsupported` |
| library hint with no library | `ws2_32` unavailable | `package` |
| include not found anywhere | `config.h` | `unknown` (error) |

Each target (toolchain+system) declares what it provides:
`provides: [platform:win32, c89, package:...]`. A unit whose requirements
exceed `provides` is **omitted** with a report line:

```
omit src/ftp/ftpcopy.c: needs platform:posix (unistd.h) – target bcc55 provides platform:win32
```

`--missing=skip|warn|error`: `skip` (default for platform/package
requirements), `warn` (build it anyway, expect errors), `error` (stop).
`unknown` is always an error unless `skip` is explicit. A **program** is
omitted when any unit it needs is omitted or any module it links has no
objects; the report names the chain.

### 5.7 Module closure

As today: a module links the modules its units name; cycles are legal
(archives listed twice or `--start-group`, depending on capability
`archive-cycles`).

### 5.8 Library hints

Hints, in the order of preference:

1. project file: `link = ["ws2_32", "shlwapi"]` per module/program;
2. pragmas: `#pragma comment(lib, "x.lib")` collected as `P<name>`;
3. header table in the descriptor: `winsock2.h → ws2_32`, `shlwapi.h → shlwapi`,
   `math.h → m` (system-dependent), `pthread.h → pthread`.

Applied by capability `pragma-comment-lib`: when the compiler honors the
pragma the hint is *reported* only; otherwise it becomes `-l`/`.lib` on the
link line (`-lws2_32`, `ws2_32.lib`, bcc55's import library from the psdk
directory).

### 5.9 Link-time and compile-time reports

`--report` writes (and `--doctor` shows) things the build will hit:
undefined-symbol prediction from the scan (functions declared in a project
header but defined by no unit), C standard mismatch (a toolchain with
`c-standard: c89` plus a unit whose scan shows `<stdint.h>`, a missing-header
for `inline`), duplicate definitions between modules. Prediction is
best effort and labelled as such.

### 5.10 Diagnostics policy

The preprocessor's diagnostics during a scan are collected. Default output is
a count per class; `-v` adds the first of each class; `-v -v` everything.
`#warning`, "extra token after #endif", unterminated conditionals at EOF are
classes. A scan error never stops generation; it marks the unit
`scan-failed` and falls back to the text scanner for that unit, with a
report line.

### 5.11 Text scanner fallback

When no compiler descriptor exists, the `#include "..."` text scanner is
used, with `-I` directories, nested includes and `#ifdef` ignored. It also
serves `-f` and `-M`. Result lines are the same `D`/`H`/`L`.

---

## 6. Toolchain descriptors

A toolchain is data. The format is the INI dialect of `lib/ini` (sections,
`key = value`, `;` comments, repeated keys append). Descriptors are compiled
into the binary (generated from `toolchains/*.tc` at build time); a file of
the same name in `$GENMAKEFILE_PATH`, `~/.config/genmakefile/` or `./.genmakefile/`
overrides it, so a new compiler needs no recompilation.

### 6.1 Structure

```ini
[toolchain]
name        = gcc
family      = gnu              ; inherit other descriptors: extends = gnu
extends     =
kind        = compiler         ; compiler | firmware | translator
c-standard  = c11              ; c89 c99 c11 c17 – property, used by 5.9
languages   = c c++ asm

[commands]                     ; templates, see 6.3
cc          = $(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
ar          = $(AR) rcs $@ $^
ld          = $(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

[names]
object      = .o
archive     = lib%.a           ; % = module name
program     = %                ; .exe via [system.windows]
shared      = lib%.so

[flags]                        ; keyed by the logical request
debug       = -g -O0
release     = -O2
minsizerel  = -Os
relwithdebinfo = -O2 -g
include     = -I%
define      = -D%
libdir      = -L%
lib         = -l%
output      = -o %
depfile     = -MMD -MF %       ; capability depfile: yes

[predef]
mode        = probe            ; probe | table | none
probe       = $(CC) -dM -E -x c /dev/null
sysinclude  = $(CC) -E -Wp,-v -x c /dev/null

[capability]
depfile = yes
pragma-comment-lib = no
response-file = no
archive-cycles = start-group
```

### 6.2 Inheritance and variants

`extends` merges sections key by key (later wins, `+=` form appends). Variants
are ten-line files:

```ini
; bcc56.tc
[toolchain]
name = bcc56
extends = bcc55
c-standard = c89
[predef.table]
__BORLANDC__ = 0x0560
```

`clang` extends `gnu`, so do `tcc`, `icc`, `avr-gcc`; `clang-cl` extends `msvc`.
`dmc`, `lcc`, `pocc`, `occ`, `zapcc`, `htc` follow today's `compiler_types[]`.

### 6.3 Command templates

Templates use make-neutral placeholders; the backend maps them:

| placeholder | meaning |
| --- | --- |
| `$@` `$<` `$^` | target, first prerequisite, all prerequisites |
| `$(VAR)` | variable defined in the IR |
| `%{objs}` | prerequisite list in a response file when the length exceeds `limit` |
| `%{inc}` | the include path, each element through `[flags] include` |

Long command lines: `[capability] response-file = @%` (gcc, ld), `ilink32 @file`
(Borland inline files `@&&|`), `link @file`. `limit = 8000` for cmd.exe,
`limit = 32000` elsewhere; above the limit the backend writes the list in a
response file *generated by the make file*.

### 6.4 Rules (what flag lists cannot express)

Some toolchains need steps. They are `[rule.NAME]` sections:

```ini
; xc8.tc – compile from inside obj/, link with --codeoffset, make .cfg
[toolchain]
name = xc8
kind = firmware
c-standard = c99

[names]
object   = .p1
program  = %.hex               ; primary output
outputs  = hex cof elf cfg     ; produced siblings (%.cof, %.elf, %.cfg)

[commands]
cc   = cd $(OBJDIR) && $(CC) --pass1 --chip=$(CHIP) $(CFLAGS_REL) $(CPPFLAGS_REL) -o$(@F) $(<REL)
ld   = $(CC) --chip=$(CHIP) $(LDFLAGS) $(if $(CODE_OFFSET),--codeoffset=$(CODE_OFFSET)) \
       --output=-mcof,+elf -o$@ $^

[rule.cfg]                     ; extra rule after link
target  = $(BINDIR)/$(PROGRAM)_$(BUILD_ID).cfg
depends = $(BINDIR)/$(PROGRAM)_$(BUILD_ID).hex
command = piccfg $< > $@

[rewrite]
include-relative-to = objdir   ; every -I becomes -I../.. when the compile runs in obj/

[predef.table.chip]
18f25k50 = PIC18F25K50=1 __18f25k50=1 _18F25K50=1
...
```

`$(<REL)` and `$(CFLAGS_REL)` are the source/flag lists with paths
rewritten relative to the compile directory (`include-relative-to`), the
mechanism that today lives in `compiler.c` for XC8. The generic form: a
toolchain may declare `compile-cwd = objdir|srcdir|root` and the generator
rewrites all path-valued flags and arguments.

```ini
; sdcc.tc – -c per source, -mpic14/-mpic16, generated .lkr, hex filter
[toolchain]
name = sdcc
kind = firmware
c-standard = c99
[flags]
arch.pic14 = -mpic14 -p$(CHIP)
arch.pic16 = -mpic16 -p$(CHIP)
[names]
object  = .rel
program = %.hex
[commands]
cc = $(CC) $(ARCH) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
ld = $(CC) $(ARCH) $(LDFLAGS) $(if $(CODE_OFFSET),-Wl-s$(LKR)) -o $@ $^
[rule.lkr]
when    = $(CODE_OFFSET)
target  = $(OBJDIR)/$(CHIP)_$(CODE_OFFSET).lkr
depends = $(GPUTILS_LKR_DIR)/$(CHIP).lkr
generate = sed 's/^CODEPAGE   NAME=vectors .*/CODEPAGE NAME=vectors START=$(CODE_OFFSET) .../' $< > $@
[rule.hexfilter]                 ; awk strips hex records below the offset
when    = $(CODE_OFFSET)
target  = $@
command = awk -f $(SCRIPTDIR)/hexstrip.awk -v offset=$(CODE_OFFSET) $@ > $@.tmp && mv $@.tmp $@
[select]
arch = chip~^1[26]f  → pic14 ; chip~^18f → pic16
```

### 6.5 Generated artifacts

Rule kind `generate` creates a file from other files with a command; both
dependency edge and command are in the IR. Used for `.lkr` (SDCC), `.def`
files, `.rc` resource compilation, embedded blobs, lex/yacc, version headers.
The generated file's path is under the object directory, never the source
tree.

### 6.6 Platform data

`[system.windows]`, `[system.linux]`, `[system.darwin]`, `[system.android]`,
`[system.none]` (bare metal) override names, flags and capability. Host/
target are separate: `--system` selects the target section. `[provides]`
(5.6) lives here:

```ini
[system.windows]
program = %.exe
provides = platform:win32 api:winsock
[provides.bcc55]
provides = platform:win32 c89 no:stdint no:inline
```

### 6.7 Detection and selection

`-t NAME` selects. Without it the tool guesses from `-o`, `-d`, `--cross`,
`CC` in the environment and the first of `gcc cc clang` on `PATH` (today:
partly). Guess is printed. `--list-toolchains` shows names and sources
(built-in or file). `--doctor` (12.3) checks that each command in
`[commands]` exists on `PATH` or is reachable under wine.

### 6.8 Toolchains to ship

| group | names |
| --- | --- |
| native C | gcc clang tcc icc msvc clang-cl dmc lcc pocc occ zapcc |
| Borland/Embarcadero | bcc55 bcc56 bcc102 (bcc32c) |
| Microchip | xc8 v1 (`xc8-v1`), xc8 v2+ (`xc8-v2`), mcc18 (data only), xc16 xc32 |
| PIC/8-bit open | sdcc (pic14 pic16 mcs51 stm8 z80), gputils (MPASM) |
| AVR | avr-gcc |
| ARM | arm-none-eabi-gcc clang-arm armcc |
| libc variants | gcc+musl gcc+diet gcc+uclibc (same family, `[libc]` section) |
| old/other | cc65 z88dk tinycc-win32 |

Only the first three groups and `xc8`/`sdcc` are in scope for the first
release (15); the others are descriptors waiting for a tester.

---

## 7. Backends

### 7.1 Capability table

Every backend declares capabilities; the generator chooses strategies from
them. Unknown means **no**.

| capability | gmake | posix make | ninja | nmake | borland make | shell | batch | MPLAB X |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| variables | ? | ? | var | yes | yes | yes | set | project |
| `?=` overridable | yes | no | no | no | no (`!ifndef`) | `${X:-}` | `if not defined` | n/a |
| conditionals | `ifeq` | no | no | `!if` | `!if` | `if` | `if` | n/a |
| nested macro expansion | yes | yes | n/a | yes | **no** | yes | n/a | n/a |
| pattern rules | `%` | suffix | n/a | inference | inference | n/a | n/a | n/a |
| order-only prerequisites | yes | no | `\|\|` | no | no | n/a | n/a | n/a |
| response files | via `$(file )` | no | `rspfile` | `@<<` | `@&&\|` | n/a | n/a | n/a |
| depfile include | `-include` | no | `deps = gcc` | `!include` | no | n/a | n/a | n/a |
| mkdir as prerequisite | reliable | reliable | automatic | reliable | **unreliable** | n/a | n/a | n/a |
| multiple outputs | grouped targets | no | yes | no | no | n/a | n/a | n/a |
| phony | `.PHONY` | `.PHONY` | phony | `.PHONY` | pseudo | n/a | n/a | n/a |
| parallel | `-j` | no | native | `/K`-ish none | none | `&` | none | IDE |
| cd in recipe | yes | yes | yes | yes | yes | yes | yes | n/a |

(`?`: GNU make has them; POSIX 2024 added `?=` — treated as unknown on
older systems; `--make-flavor posix` enforces the lowest common set.)

### 7.2 Strategies chosen from capability

| need | strategy |
| --- | --- |
| overridable variable | `X ?= v` / `!ifndef X` + `X = v` + `!endif` / `X=${X:-v}` / `if not defined X set X=v` |
| all build types in one file | variable-selected flag sets when *nested macro* is yes; otherwise `--all-types` writes one file per type, or a `!if "$(BUILD)"=="Debug"` chain (borland) |
| object directory | order-only prerequisite; otherwise a `mkdir` command in the object rule (`if not exist D mkdir D`) |
| header dependencies | build-time depfile where supported (gcc/clang `-MMD`, msvc `/showIncludes`, ninja `deps`), else the static `D` lines from the scan |
| long link lines | response file |
| multiple outputs (xc8 `.hex/.cof/.elf`) | grouped target / stamp file for the primary and `touch`-ed siblings |
| clean | `$(RM) list` of what the graph knows (never `rm -rf $(BUILDDIR)` unless asked) |

### 7.3 Per-backend notes

* **gmake**: default. `.DELETE_ON_ERROR:`, `.SECONDEXPANSION` avoided,
  `$(MAKEFILE_LIST)` for the regeneration rule.
* **posix**: stricter subset; no `$(shell)`, no `%` rules; suffix rules only.
* **ninja**: `build` edges, `rule` per action; `restat`, `pool console` for
  link; `compdb` via `ninja -t compdb`. Regeneration through `generator = 1`.
* **nmake**: `!IF`, `!INCLUDE`, `{src}.c{obj}.obj:` inference rules; `/NOLOGO`;
  `cl /showIncludes` filter script for depfiles (optional).
* **borland**: today's output; capability quirks of 1.3 (inline file at col 0,
  `tlib` `+"x"`/`-+`, no nested macros, `-D` overridable only with `!ifndef`).
* **shell**: one script, `set -e`, functions per action, timestamps via
  `[ target -nt src ]`; no dependency graph beyond that.
* **batch**: `cmd.exe` with `if not exist`, `call :label`; for toolchains with
  no make (xc8 on a stock Windows).
* **MPLAB X**: `nbproject/configurations.xml` + `Makefile-<conf>.mk` (today:
  `mplab.c`, `mplab-nbproject.txt`). Treated as an *export* target: it writes
  the project the IDE generates from; building is MPLAB's (`prjMakefilesGenerator`).
* **cmake**: `CMakeLists.txt` as export (today: `cmake.c`), for IDEs; not
  expected to round-trip.
* **compile_commands.json**: always available with `--compdb`, any backend.

### 7.4 Regeneration rule

The make file starts with a rule that regenerates itself when the project
file, a descriptor, the genmakefile binary or the *set of source files*
changes:

```make
Makefile: $(GENMAKEFILE_DEPS)
	genmakefile @Makefile.cmd -o $@
```

`@FILE` is a response file with the original arguments, written next to the
output (`.genmakefile/<out>.cmd`). The set of source files is a directory
stamp (`.genmakefile/<out>.list`, sorted names, compared by content, so adding
a file triggers regeneration but touching one does not). Backends without
the capability omit the rule and say so in a comment.

### 7.5 Header dependencies at build time

Static `D` lines are exact for the point scanned but go stale when a header
adds an include. Where the compiler can emit depfiles the generated file uses
them *in addition*: first build uses the static list, later builds use the
depfile, and both are merged. `--deps=static|depfile|both` (default `both`
when the capability is present).

### 7.6 Install, uninstall, dist, check

See section 10; those are targets of the IR, not of a backend.

---

## 8. The build matrix

### 8.1 Axes

A **point** is one value from each axis that applies:

| axis | option / project key | example |
| --- | --- | --- |
| toolchain | `-t` / `compilers` | `xc8`, `sdcc` |
| chip | `-p` / `chips` | `18f25k50`, `16f1829` |
| build type | `--debug` etc. / `types` | `debug`, `release` |
| crystal | `--xtal` / `xtals` | `20000000`, `48000000` |
| baud | `--baud` / `bauds` | `9600`, `38400` |
| code offset | `--code-offset` / `offsets` | `0x0000`, `0x2000` |
| program | discovered | `rgbtest`, `LC-meter` |
| switch | `-DUSE_X` / `switch.X` | `USE_USB` on/off |
| system | `-s` | `linux`, `windows` |

*(today)*: every axis except switch and system has an option; the
combinations are written one file per point.

### 8.2 Expansion and the driver

`--matrix FILE` (or the project's `[matrix]`) lists the values per axis and
constraints:

```ini
[matrix]
compilers = xc8 sdcc
chips.xc8  = 18f25k50 18f26j50 16f1829
chips.sdcc = 18f25k50 16f1829
types      = debug release
xtals      = 20000000 48000000
bauds      = 9600 38400
offsets    = 0x0000 0x2000
exclude    = chips=16f1829 offsets=0x2000     ; no bootloader on 16f
```

`genmakefile --matrix m.ini --outdir mk/` writes one make file per point
(`mk/<compiler>-<chip>-<build id>.mk`, user decision kept) **and** a driver:

```make
# mk/Makefile  (generated)
POINTS := xc8-18f25k50-release_20mhz_9600bps xc8-18f25k50-release_20mhz_38kbps ...
all: $(POINTS)
$(POINTS):
	$(MAKE) -f mk/$@.mk
.PHONY: all $(POINTS) clean
```

reproducing the loops of pictest's top level `Makefile` (`COMPILERS`, `CHIPS`,
`BUILD_TYPES`, `XTAL_FREQS`, `BAUD_RATES`, `CODE_OFFSETS`, `PROGRAMS`). The
driver accepts the same override variables: `make CHIPS=18f25k50 BUILD_TYPES=debug`
filters `POINTS` (generated as `$(filter ...)`).

### 8.3 Build id

Normative: `<type>_<mhz>mhz_<kbps>kbps[_at<offset>]` (as the hand written
`build/vars.mk`):

* `type`: lower-case build type.
* `mhz`: `xtal / 1000000` with trailing `.0` dropped (`1`, `4`, `20`);
  `48000000` with PLL descriptor `internal-osc` is `intosc48`; the rule is a
  per-chip descriptor entry (`[chip.18f25k50] 48000000 = intosc48`), not a
  special case in code.
* `kbps`: `baud / 1000` with at most 3 decimals dropped (`9600` is `9.6`,
  `38400` is `38.4`; written `9_6`? **no**: `9.6kbps`, with the dot kept since
  both make and the file systems of the targets allow it).
* `_at<offset>`: only for non-zero offset, hex as written (`0x2000`).

The format string is a descriptor key (`[names] build-id = ...`) so that
lc-meter, which derives type from `_DEBUG` and uses `XTAL`, can differ.

### 8.4 Layout

```
obj/<compiler>-<chip>/<build id>/<program>/<source>.<obj>
bin/<compiler>-<chip>/<program>_<build id>.{hex,bin,cof,elf,cfg}
```

as a `[layout]` template of the firmware kind; the generic kind uses the
layouts of 4.6. A project can set `layout = "..."` to any template using
`$(compiler) $(chip) $(build_id) $(program) $(type)` (not make variables:
resolved at generation).

### 8.5 Constraints

* `exclude`/`only` clauses as above;
* per-chip capability (e.g. `usb = yes`, `flash = 32k`, `lcd = no`) turns
  switches on or off *per point*: `USE_HD44780_LCD` off on `12f1840` and
  `16f628a`, `USE_USB` implies the usb sources and `EP*_BUFFER_BASE_ADDR` defines;
* a baud/xtal combination can be rejected by a function (`error if
  |baud_actual - baud| > 3%`), a standard rule for `BRG16`; reported, not
  silently adjusted.

### 8.6 Without a matrix

A single point: all values from the command line, same code path.

---

## 9. Firmware, packages, probes

### 9.1 What firmware changes

| aspect | desktop | firmware |
| --- | --- | --- |
| entry | `main(int, char**)` | `void main(void)` and an ISR; no OS |
| outputs | executable | `.hex` (+`.cof`, `.elf`, `.cfg`, `.map`) |
| libc | provided | vendor, chip dependent, tiny |
| predefs | from the compiler | from the compiler **and** the chip |
| compile cwd | anywhere | XC8 compiles from inside `obj/` |
| link | `-l` | `--runtime`, `--codeoffset`, linker script |
| tests | run | simulator or none |

All handled by descriptor kinds (6), not by a firmware mode. The knowledge
collected from the author's build systems is in `doc/genmakefile-toolchains.md`;
draft descriptors are in `src/genmakefile/toolchains/`.

**Policy (author, 2026-11-01): every supported compiler stays supported and is to
be improved; only HI-TECH C (`picc`/`picc18`, discontinued, now XC8) was dropped.**

### 9.2 PIC projects (the authors reference case)

Facts (from TASKS, normative for the conformance fixtures, 14.4):

* `pictest`: mains in `src/*test.c`, shared code in `lib/` (the libpicp
  submodule, **flat**), per-program source lists and defines
  (`<prog>_SOURCES`, `<prog>_DEFS`), chip dependent additions; defines every
  build gets: `PIC<CHIP>=1 __<chip>=1 _XTAL_FREQ=<hz> UART_BAUD=<baud>`,
  debug adds `__DEBUG=1 _DEBUG=1`, release `NDEBUG=1 __NDEBUG=1`;
  include path `-I. -Ilib -Isrc`.
* `lc-meter`: mains at the root (`LC-meter.c`, `Cap-meter.c`, `Freq-meter.c`),
  helpers in `src/`, `XTAL` instead of `_XTAL_FREQ`, build type derived from
  `_DEBUG`, a chip list per compiler.

Project file for pictest (the rest is derived by scan):

```ini
[project]
name     = pictest
layout   = firmware
roots    = src lib
include  = . lib src
program-glob = src/*test.c

[define]
all      = PIC$(CHIP_UPPER)=1 __$(CHIP)=1 _XTAL_FREQ=$(XTAL) UART_BAUD=$(BAUD)
debug    = __DEBUG=1 _DEBUG=1
release  = NDEBUG=1 __NDEBUG=1

[matrix]                         ; as 8.2
...

[switch.USE_USB]
kind     = flag
sources  = lib/usb/*.c
define   = EP1_BUFFER_BASE_ADDR=0x500 EP2_BUFFER_BASE_ADDR=0x540
only     = chips=18f25k50 18f26j50

[switch.USE_HD44780_LCD]
kind     = flag
exclude  = chips=12f1840 16f628a
```

What the scan contributes and the project file does not need to say: which
lib units each program links (from its `#include`s), which `USE_*` exist,
which headers a unit depends on per chip.

For lc-meter:

```ini
[project]
name = lc-meter
layout = firmware
roots = . src lib
program-glob = *.c            ; mains at the root; the scan keeps those with main
[define]
all  = XTAL=$(XTAL) PIC$(CHIP_UPPER)=1
[layout]
build-id = $(type)_$(mhz)mhz
```

### 9.3 Code offset (bootloader) variants

A bootloader build links two images: the application at an offset and the
bootloader at 0. Both are matrix points differing in `offset`. For XC8,
`--codeoffset=<hex>` is passed when non-zero. For SDCC the descriptor's
`[rule.lkr]`/`[rule.hexfilter]` (6.4) produce the linker script and strip
the hex records below the offset; `ivt-loc=offset+8` is a descriptor
expression. Both are tested by comparing generated command lines with
`make -n` of the hand written build (14.4).

### 9.4 Optional packages

Package = (name, header(s), library(s), variable names, pkg-config module).

```ini
[package.zlib]
header   = zlib.h
library  = z
pkgconfig = zlib
define   = HAVE_ZLIB=1
variable = LIBZ                ; make variable that enables it
```

Resolution order at generation: `--dep`/project file → built-in table
(brotli, bzip2, lzma, zlib today) → `pkg-config --cflags --libs` if the host
has it **and** the point is native. The generated file always leaves the
package off until the variable is set (decision kept), but prints the
chosen strategy in a comment:

```make
# zlib: enable with  make LIBZ=z [LIBZ_INCLUDEDIR=... LIBZ_LIBDIR=...]
```

`--packages=auto` additionally runs a probe (9.5) so a developer machine gets
the package turned on when it works.

### 9.5 Probes (replace `config.h`)

A probe compiles (and optionally runs) a small program with the point's
toolchain to answer a question. Results become defines `HAVE_X=1` in a
generated header or `-D` flags.

```ini
[probe.HAVE_ALLOCA]
source  = #include <alloca.h>\nint main(){return alloca(1)!=0;}
need    = compile
[probe.SIZEOF_LONG]
source  = #include <stdio.h>\nint main(){printf("%d",(int)sizeof(long));return 0;}
need    = run              ; skipped when cross compiling, then 'default = 4'
default = 4
```

Probes run at **generation** (when the toolchain is present and
`--probe` given), cached by compiler identity, and the generated header
`config.h` is written beside the output; or at **build** time as make
rules that generate it, for a generation host without the compiler (needs
the backend capability `generate`; the probe shell scripts are emitted).
For bcc55 the probe set is small and listed in the descriptor; for firmware
`need = compile` only.

### 9.6 Cross-compiling and wine

`--cross PREFIX` for gnu; `runner = wine` in the descriptor for Windows
toolchains on Linux; `WINEPATH` set from `[env]`; `--doctor` checks that
wine starts. A probe with `need = run` uses the runner or its default.

---

## 10. Targets

Standard targets of the IR (names configurable):

| target | content |
| --- | --- |
| `all` | programs and libraries of the role `program`/`module` |
| `libs`, `bins`, `objs` | the three `--create-*` groups |
| `check` | build and run `test` role programs; exit status aggregated; for firmware: simulator command if the descriptor has one |
| `examples` | the `example` role |
| `install` / `uninstall` | `DESTDIR`, `PREFIX`, `BINDIR`, `LIBDIR`, `INCLUDEDIR` honoured; programs, libraries, `--header`s; directories created |
| `dist` | tar/zip of sources named by the scan plus project files (listed, not copied from `git`) |
| `clean` | remove what the graph knows; `distclean` also generated make files and caches |
| `compile_commands.json` | `--compdb` |
| `tags` | optional `ctags` over the scanned files |
| `<program>` | one target per program, so `make rgbtest` works |
| `help` | lists targets, switches and packages with their defaults |

Firmware adds: `flash` (descriptor command, e.g. `pk2cmd -P... -F$(HEX)` or
`ipecmd`), `size` (print memory use from the map), `cfg` (the `piccfg` rule).

### 10.1 Variables every generated file honours

`CC CXX AR RANLIB LD RM MKDIR CP`, `CFLAGS CXXFLAGS CPPFLAGS LDFLAGS LIBS`,
`DESTDIR PREFIX`, `BUILD` (type), `V=1` (echo commands, default quiet `@`
prefix), `JOBS` where meaningful, plus every switch, package and matrix
axis as overridable variable. Overriding `CFLAGS` appends to the generated
warnings and includes, `OVERRIDE_CFLAGS` replaces.

---

## 11. Interface

### 11.1 Command line

Stable subset (all options of today's `doc/genmakefile.md` stay):

```
genmakefile [options] [--] ROOT...
genmakefile --project FILE [options]
genmakefile --matrix FILE --outdir DIR
genmakefile --doctor | --list-toolchains | --list-layouts | --report
genmakefile -f BUILDLOG | -M CMAKEDIR            (input adapters)
```

Exit codes: `0` ok, `1` usage, `2` generation error, `3` generated with
omissions (so scripts can tell `--missing=skip` happened), `4` internal.

### 11.2 Project file

INI dialect (same parser as descriptors; keeps the tool free of new
dependencies). Keys of the common sections:

| section | keys |
| --- | --- |
| `[project]` | `name layout roots exclude include define program-glob lib-dir naming objects toolchain default-target` |
| `[module.NAME]` | `sources headers link requires skip` |
| `[program.NAME]` | `sources link define switches skip` |
| `[role]` | `glob = role` pairs (`tests/*.c = test`) |
| `[define]` | `all debug release <switch>` |
| `[switch.X]` | `kind default define sources only exclude` |
| `[package.X]` | see 9.4 |
| `[probe.X]` | see 9.5 |
| `[matrix]` | see 8.2 |
| `[toolchain.NAME]` | override or extend a descriptor inline |
| `[install]` | `headers programs libraries prefix` |

Project file wins over detection; detection wins over defaults; command line
wins over all. `genmakefile --dump-project` prints the effective merged result
(with the origin of each value), so the answer to "why did it build this" is
one command.

### 11.3 Environment

`GENMAKEFILE_PATH`, `GENMAKEFILE_CACHE`, `CC`/`CXX`/`AR` (used for guessing
and probes, never baked into the output unless `--freeze-tools`).

### 11.4 Machine-readable output

`--format=json` for `--report`, `--doctor`, `--dump-project`; `--compdb`
for the compile database. Fields are stable (versioned by `"schema": 1`).

### 11.5 Reproducibility

`--reproducible` removes the command line comment, uses forward slashes in
comments, sorts everything. Checked by the suite: generating twice and on two
hosts must be bit identical.

### 11.6 Cache

See 13.2.

---

## 9 (continued). Inferring firmware builds from the sources

Sections 9.7 to 9.14 come from reading the five repositories of the author
(all public, cloned 2026-11-01 for this specification):

| repo | what it is | build today |
| --- | --- | --- |
| `pictest` | test programs for many PICs; mains in `src/`, config headers `src/config-<chip>.h`, submodule `lib` = libpicp, submodule `usb` = USB-Stack | top level `Makefile` + `build/{vars,xc8,sdcc,htc,mcc18,rates,common}.mk`, `gen-mk.sh` (xc8/htc/sdcc x make/ninja/nmake/mplab), MPLAB 8/X trees, Code::Blocks files |
| `lc-meter` | LC meter firmware: mains `LC-meter.c`, `Cap-meter.c`, `Freq-meter.c` at the root, helpers in `src/`, libpicp as `lib/` | same `build/*.mk` family, plus **XC8 argument files** (`LC-meter-debug.xc8`: `--chip=… -Ilib -Isrc -D…`), MPLAB trees |
| `libpicp` | the flat library (24 `.c` + `extra/`); not a project | `Makefile.{htc,sdcc,xc8}`, `gen-ninja-build.sh` |
| `usb-stack` | USB device stack (`USB_Stack/USB/*.c`, `Hardware/`) and 8 example projects | **MPLAB X only**: `*.X/nbproject/configurations.xml` with 2 to 10 chip configurations each; examples share `Shared_Files/` through `sourceRootList` |
| `usb-uc` | USB bootloader firmware, 11 chips; assembly variant | MPLAB X (`USB_uC.X`, 4 test projects, one MPASM project) |
| `an-tronics` | hardware (Eagle/KiCad/Fritzing) and a host tool `calc-lc-tank.c` | `build/gerbers.mk` only; **no firmware**: the layout `hardware` (4.2) exists so that genmakefile ignores it correctly |

What follows is the specification of "infer build parameters from the
sources": which parameter comes from where, and what to do when it is not
there.

### 9.7 Parameter sources, in priority order

| parameter | sources, highest first | observed in |
| --- | --- | --- |
| **chip(s)** | `--chip`; project/matrix; MPLAB X `conf`/`targetDevice`; `.xc8` argfile `--chip=`; `src/config-<chip>.h` file names (the set of chips the code *supports*); `#if defined(_16F876A)` ladders in config headers; `CHIPS` in `build/vars.mk` | pictest has 14 `config-*.h` (12f1840 to 18f25k50, 18f16q41); lc-meter's `CHIPS` differs per compiler; usb-stack examples 2 to 10 confs |
| **toolchain and version** | `--toolchain`; project; MPLAB X `languageToolchain` + `languageToolchainVersion` (`XC8` `1.43`); `CCVER` of the make files (per program, `seg7test_CCVER = 9.83`/`9.80` by chip); first compiler found | `v1.43` default, `v2.36` for 18f16q41, SDCC 3.6/4.6 |
| **clock** (`_XTAL_FREQ`, `XTAL`) | option; matrix; `#define`s in config headers (`XTAL_USED`, `NO_XTAL`, `_XTAL_FREQ` defaults); the board profile (9.12); default 20 MHz | 48 MHz forced by `USE_USB`; picstick has no crystal (`intosc48`) |
| **baud** | option; matrix; `UART_BAUD`/`SOFTSER_BAUD`/`MIDI` constraints; default 38400 | `BAUD=31250` required for MIDI programs, 9600/19200/38400 sweeps |
| **code offset** | option; matrix; MPLAB X `additional-options-code-offset`; bootloader project name (`_boot`, `bootloaders/`); default 0 | `0x0000 0x0800 0x1000 0x2000` |
| **defines** | option; project; switches found by scan (5.4); MPLAB X `define-macros`; argfile `-D`; generated `PIC<CHIP>=1 __<chip>=1` | `USE_TIMER0=1`, `SPI_USE_HW=1`, `NO_PLL`, `EP*_BUFFER_BASE_ADDR` |
| **include path** | option; project; `INCLUDE_DIRS`; MPLAB X `extra-include-directories`; argfile `-I`; roots of the project; **sibling checkouts** (9.11) | `-I. -Ilib -Isrc`, `../USB-Stack/USB_Stack/{USB,Hardware}` |
| **sources of a program** | project `[program.X]`; MPLAB X `itemPath` list; make `X_SOURCES`; **inference** (9.8) | `rgbtest_SOURCES += usb.c …` under `USE_USB` |
| **optimisation, warnings, runtime, linker options** | toolchain defaults per build type; MPLAB X `HI-TECH-COMP`/`HI-TECH-LINK` property lists (`optimization-level -Os`, `warning-level -3`, `code-model-rom`); argfile | `--opt=default,+asm,+debug,+speed,-space,9`, `--runtime=default,+init,+osccal,+download,+clib`, `-N127`, `--float=24 --double=24`, `--stack=compiled` |
| **outputs** | toolchain: `.hex .cof .elf .cfg .map .sym` | `piccfg` writes `.cfg` |
| **programmer / flash** | project `[flash]`; `README.ipecmd`, `pk3cmd.sh`, `program.sh` are *not read*; documented in the project file | pickit3, ipecmd, pk2cmd |

Everything has a fallback; nothing blocks on a missing source. Every value in
the end carries its **origin** (`--dump-project` shows it, 11.2): a build
whose chip came from `config-` file names says so.

### 9.8 Source inference by symbol closure

*Problem.* The hand lists (`pictest_SOURCES = pictest.c lib/delay.c lib/lcd44780.c
lib/ser.c …`) are what the author maintains and what genmakefile should make
unnecessary. Mapping `#include "lib/foo.h"` to `lib/foo.c` is not enough, and
measuring shows by how much (experiment of 2026-11-01, `pictest` at
18f25k50 with the per-program `_DEFS`, 11 programs, 65 units listed by hand):

| method | matches the hand list | misses | extras |
| --- | --- | --- | --- |
| include name mapping, unconditional (naive text scan) | 46 | 16 | 34 |
| include mapping after preprocessing with the point's defines | 45 | 17 | 7 |
| **symbol closure after preprocessing** (this section) | 30 *non-empty* | 30 *of which 25 compile to nothing* | 5 |

Reading the last row: of the 30 listed units the closure does not select, 25
produce **no code at all** at that point (`lib/uart.c` with `USE_UART` off,
`lib/lcd44780.c` without `USE_HD44780_LCD`, `lib/adc.c`, `lib/delay.c`…): the
hand list links empty objects, and leaving them out changes nothing. 5 are
real: `buffer.c` (`buffer_*` is reached through a macro the simple scan did not
see), `7segment.c`, `timer.c` for `pwmtest`, i.e. the first-cut identifier
matcher had holes, not the idea. The 5 extras (`ds18b20.c`, `ledsense.c`,
`shell.c`, `comparator.c`) are units that **define** a symbol the program text
**mentions** (a declaration, a macro argument) without calling it: harmless
for correctness, costing flash only if the linker does not discard them, which
XC8/SDCC do for unreferenced functions. Conclusion: the idea is right and
cheap, include mapping is not.

*The algorithm* (the static-archive rule, which is the diet-libc rule applied
to the user's own sources):

1. Preprocess every unit under `lib` roots with the point's predefs and
   defines (the scan, 5.1) -- once per *point*, cached by (unit hash, defines
   hash).
2. For each unit compute **D(u)** = external symbols it defines (functions and
   variables at brace depth 0 that are not `static`; `_at`/`__at()` definitions
   included; ISRs `void interrupt isr()`/`__interrupt` count as defined) and
   **R(u)** = identifiers appearing in its preprocessed text that are not
   locally declared.
3. Start from the main unit. A unit `v` is **needed** when `D(v) ∩ R(u)` is
   non-empty for any needed `u`. Iterate to a fixed point.
4. A unit that defines something but is never referenced is **not linked**
   (and reported with `-v` as `unused`).
5. Units without a `D(u)` (empty at this point) are **skipped and reported**
   as `empty`, never compiled, so objects are saved too.
6. Ambiguity (two units define the same symbol, e.g. `lib/ser.c` and
   `lib/softser.c` both provide `ser_*` by switch): both stay in the closure
   when both are non-empty and the scan warns `duplicate symbol` -- the build
   would fail at link time anyway; the switch decides which is non-empty.

Identifier matching is by token, not by regex, using `lib/cpp`'s tokenizer;
function-like macros are already expanded by the preprocessing so the
`buffer_*` holes above close. Cost: the tokenizer processes each unit once per
point; the whole of libpicp is 24 units.

Additions that cannot be seen in code are given in the project file
(`[program.X] sources = …`, `[switch.USE_USB] sources = …`); `exclude =`
removes a unit the closure adds. A project may turn inference off
(`infer = headers|closure|none`), `headers` being the weaker mapping that is
useful on tools without a tokenizer budget.

### 9.9 Chip-dependent configuration found in the sources

`pictest/src/config-<chip>.h` is the one place that holds fuses
(`#pragma config`) and clock selection; `src/config-bits.h` picks one by
`#include "config-" CHIPNAME ".h"`-style ladders. The scan treats a header
named `config-<token>.h` as a **chip configuration**: the set of `<token>`s is
the *supported chip list* of the project (9.7 row 1), and the unit including
it becomes chip-dependent. A chip whose config header is absent is reported
as `unsupported` rather than built into a link error. `#pragma config`
lines are recorded but not interpreted (the compiler does that); the report
lists which fuses each chip configuration sets, useful in review.

### 9.10 MPLAB X projects as input

14 of the 20 firmware projects in the five repositories are MPLAB X
projects (`*.X/nbproject/{project.xml,configurations.xml}`), with no
hand-written makefile. Their configurations are rich and machine readable, so
they are a **first-class input adapter** (`genmakefile --from-mplabx DIR`):

| `configurations.xml` | becomes |
| --- | --- |
| `confs/conf@name` (`PIC18F25K50`) | one **point** per conf (chip axis); the conf name is also the output suffix |
| `toolsSet/targetDevice` | chip (authoritative; **note**: in `usb-stack`'s CDC example the conf is `PIC16F1454` while `targetDevice` says `PIC18F25K50`; the adapter warns `conf name and device differ` and trusts `targetDevice`) |
| `languageToolchain`, `languageToolchainVersion` | toolchain `xc8` and `CCVER` (searched under `/opt/microchip/xc8/v<ver>`, `C:/Program Files*/Microchip/xc8/v<ver>`; missing version falls back to the newest installed with a warning) |
| `logicalFolder/itemPath` under `SourceFiles` | sources; `HeaderFiles` are dependencies |
| `sourceRootList/Elem` (`../Shared_Files`, `.`) | source roots, resolved relative to the `.X` directory (this is how three CDC examples share `usb_app.c` and `usb_descriptors.c`) |
| `HI-TECH-COMP` properties | compile flags: `optimization-level -Os`, `warning-level -3`, `define-macros`, `extra-include-directories`, `pack-struct`, `short-enums`, `operation-mode pro`, `asmlist`, `garbage-collect-*` mapped by the table in the toolchain descriptor (`[mplabx.map]`) |
| `HI-TECH-LINK` properties | link flags: `code-model-rom`, `additional-options-code-offset`, `clear-bss`, `data-model-size-of-double`, `display-*` |
| `packs` | recorded (DFP name/version), emitted as `--dfp=` when the toolchain is v2+ |
| `compileType/linkerTool/linkerLibItems` | libraries |
| `makeCustomization*` | pre/post steps as `[rule.pre]`/`[rule.post]` |

The output is whatever the other backends write (gmake, ninja, nmake…), so
the same project builds without the IDE, on a CI machine, for all
configurations at once -- the `usb-stack` examples have 28 configurations
in 8 projects. The *generated* `Makefile-<conf>.mk` that MPLAB X writes
next to the XML is used only as a **cross-check** in the conformance suite
(14.4): flags from the XML path and from the IDE's makefile must agree.

Unknown property keys are carried over verbatim into a comment so nothing is
dropped silently; unknown toolchain ids stop the import with a message.

MPASM/gputils assembly projects (`usb-uc/Assembly Versions/…`) import with
the `gputils` toolchain (`gpasm`, `gplink`, `.o`/`.hex`) -- same adapter,
different descriptor, no C scan (an assembly scan reads `#include`/`include`
and `INCLUDE` directives).

### 9.11 Sibling checkouts and submodules

`pictest/.gitmodules` names `lib` (libpicp), `usb` (USB-Stack), bootloader and
wave-cpp-xc8 repositories; `USB_STACK_DIR := ../USB-Stack/USB_Stack` points
at a *sibling directory*, and the checked-in `lib/` is empty without
`git submodule update`. Rules:

* roots may be **any path**, absolute or relative to the project; the output
  keeps them relative to the output file (4.7);
* `.gitmodules` is read: a root that is a declared submodule and is empty or
  absent produces `submodule 'lib' (libpicp) is not checked out` with the
  command to fix it, not "no sources found";
* a **resolver list** gives a name several places to look
  (`[roots] usb-stack = ./usb:../USB-Stack:$USB_STACK_DIR`), first existing wins;
* included headers outside the project roots but inside a declared root count as
  project headers (dependencies), those in the toolchain's directories as
  system (5.3);
* `genmakefile --fetch-deps` is a non-goal: it only reports.

### 9.12 Boards

A **board profile** answers the questions the chip cannot: crystal present
or not, pin map, LEDs, which programmer. pictest keeps this as prose
(`picstick.md`, hardware notes in `CLAUDE.md`). Profile (project file):

```ini
[board.picstick_25k50]
chip      = 18f25k50
xtal      = 48000000          ; no crystal populated: internal oscillator + PLL
define    = XTAL_USED=NO_XTAL
flash     = pickit3
```

`--board NAME` selects chip, clock, defines and `flash` in one go; the
matrix can range over boards instead of chips (`boards = picstick_25k50
generic_18f2550`). The rule "USB requires an exact 48 MHz clock" from
`vars.mk` becomes a **constraint** (8.5): `if USE_USB then xtal = 48000000`,
so `make USE_USB=1` selects it instead of silently building at the wrong clock.

### 9.13 Compiler discovery and per-program compiler versions

* Search: `$CC`; `PATH`; `/opt/microchip/xc8/v*/bin`; `C:/Program Files*/Microchip/xc8/v*`;
  `/usr/bin/sdcc`, `C:/Program Files/SDCC`; wine prefixes when the toolchain
  has `runner = wine`. Several versions installed: choose the one the
  project asks for, else the highest not above `max-version`, and say so.
* **Per-program version**: `seg7test` asks for 9.80/9.83 (HI-TECH C, dropped
  2026-11-01) while the rest uses 1.43. In the IR `CCVER` is a variable of the
  *program node*, so one make file can drive two compilers; the descriptor
  selects the command from `$(CCDIR)`.
* Flag compatibility across versions is a descriptor section per major
  (`[xc8.v1]`, `[xc8.v2]`, `[xc8.v4]`): XC8 v1 `--chip`, `--opt`, `--pass1`;
  v2+ `-mcpu`, `-O`, `-xassembler`, `-mdfp`; pack handling differs. A project
  that builds with 1.43 and 2.36 (pictest's `18f16q41`) selects by chip.

### 9.14 What "well built firmware" means (acceptance)

A build is *well built* when, besides compiling:

1. the output set is complete: `.hex` for every program and point, plus `.cof`
   (debug) and `.cfg` where the toolchain provides them;
2. names follow the layout (8.4); two points never share an output;
3. the **memory report** (`--summary=psect,mem,class,file`, SDCC `.mem`) is
   captured to `<out>.map`/`.mem` and `make size` prints it; a configurable
   budget (`[limits] rom = 95%`) fails the build when exceeded;
4. debug builds carry symbols for the debugger (`-G`, `--debugger=pickit3`),
   release builds define `NDEBUG`;
5. a bootloader offset variant has the vector table at the right place
   (`--ivt-loc`) and its hex has no records below the offset -- `make check-hex`
   runs the awk filter in verify mode;
6. rebuilding with nothing changed does nothing; changing a `config-*.h`
   rebuilds only the units that include it;
7. the reproducible flag (11.5) gives bit-identical `.hex` across runs, which
   holds for XC8 and SDCC when no `__DATE__`/`__TIME__` is used (the scan
   warns when a unit uses them).

---

## 12. Diagnostics and self-checks

### 12.1 Report

Every run ends with a summary (stderr, `-q` silences):

```
genmakefile: xc8-18f25k50 release_20mhz_38kbps  12 programs, 31 units
  omitted: 2 (src/bell.c.disabled: ignored; src/ctmutest2.c: needs chip>=18f25k22)
  inferred: 11 programs by symbol closure, 3 units empty at this point (skipped)
  switches: USE_TIMER0 USE_TIMER1 USE_SER … (14, defaults off)
  warnings: 3 (-v for details)
```

### 12.2 `--explain TARGET`

Prints why a target exists and what it depends on, with the chain: "`obj/…/timer.p1`
← `lib/timer.c` ← needed by `src/blinktest.c` (symbol `timer0_init`) ← program
`blinktest` (main found at line 41)". This is the debugging tool for 9.8.

### 12.3 `--doctor`

Checks the environment for the selected toolchain and point: compiler found
and version, make program present, wine usable, `pkg-config` present, probe
programs build, write access to the output directory, descriptor syntax. Output
is pass/fail lines with the fix.

### 12.4 Lint of the project file

Unknown keys are errors with suggestions (Levenshtein); a key that has no
effect for the selected layout is a warning.

---

## 13. Platforms and performance

### 13.1 Host platforms

Linux, macOS, Windows native (MSVC, MinGW, Borland), Cygwin/MSYS2, Android
(Termux). No dependency except the C library of the platform and the `lib/`
modules; no fork on Windows (the per-unit child process of the scan is
`process_create` of the `process` module, which abstracts `fork`/`CreateProcess`).
*(today)*: `fork`, so Windows builds of genmakefile use the in-process scan.

### 13.2 Cache

`.genmakefile/cache/` (or `$GENMAKEFILE_CACHE`): per unit and point:
key = hash(unit content, headers' content hashes from the previous scan,
defines, predefs hash); value = the scan lines (5.1). A run first checks the
header list recorded for each cached unit (stat + size + mtime, then hash on
mismatch), so editing one header re-scans only the units that included it.
Expected: whole c-utils tree no-change run < 1 s (today 7 s), pictest < 0.3 s.
The cache is a plain directory of small files, safe to delete, never read from
a different tool version (version in the key).

### 13.3 Parallelism

The scan is embarrassingly parallel (per unit); a pool of `-j N` workers
(default CPU count) with results merged in sorted order, so output stays
deterministic.

### 13.4 Memory

`lib/cpp` leaks tokens per unit (TODO `cpp-leaks`); the per-unit process boundary
makes this harmless; the thread/pool variant needs the leak fixed or an arena
per unit freed at the end (planned: `cpp_ctx` is already caller-owned,
`cpp_free` releases all tokens).

---

## 14. Verification

### 14.1 Layers

| layer | what | needs a compiler? |
| --- | --- | --- |
| unit | descriptor parser, path rewriting, build-id formatting, matrix expansion, project file merge, capability selection, INI-edge cases | no |
| golden | generate for fixture projects with every backend; compare to checked-in expected files | no |
| scan | cpp scan of fixtures with known `D/H/L/Q/V/X` lines | no |
| conformance | the reference projects (14.4) generate the same names/flags/sources as the hand-written builds | no (compare to `make -n`) |
| smoke | build a toy project with each *available* toolchain (gcc, clang, tcc, sdcc if installed) and run it | yes, skipped when absent |

### 14.2 Fixtures

`tests/genmakefile/fixtures/<name>/` with the tree, `cmd` (arguments),
`expected/<backend>/` files. Fixture set: `modular` (mini c-utils),
`flat-lib` (mini libpicp), `src-include`, `monorepo`, `firmware-xc8`,
`firmware-sdcc`, `mplabx-multi` (three confs), `vendor-tree` (zlib-like),
`collision`, `missing-include`, `submodule-empty`. (Existing: the repository's
`tests/genmakefile/testsuite.sh` is slow and drives real toolchains; it remains
a smoke layer and is **never run by default**, per CLAUDE.md.)

### 14.3 Determinism and idempotence tests

Generate twice -> identical; generate on a copy of the tree in another
directory with `--reproducible` -> identical except for configured roots;
regenerate after `touch` of every file -> identical.

### 14.4 Reference-project conformance (PIC)

Using the five repositories as fixtures (pinned commit SHAs in the fixture
file; trees checked out by a script, not vendored):

| check | method |
| --- | --- |
| object and binary **names** | for all points of `pictest` and `lc-meter`: set of paths from `genmakefile` equals the set printed by `make -n` of the hand-written build (`OBJDIR`, `BUILDDIR`, `…_$(BUILD_ID).hex`) |
| **defines** | multiset of `-D` per object equals |
| **include paths** | equal after the xc8 `-I../` rewriting |
| **source lists** | closure (9.8) vs `_SOURCES`: allowed differences are the empty units; counts reported |
| **MPLAB X** | per `.X` project and conf: genmakefile flags equal flags in the IDE's `Makefile-<conf>.mk` |
| **commands** | tokenised compile/link command per toolchain equals the hand-written one modulo ordering of independent flags (table of permitted differences in the fixture) |
| **build** (optional, if XC8/SDCC installed) | build one program per toolchain, compare the `.hex` byte for byte with the one from the hand-written build |

On this machine XC8 and SDCC are not installed, so the last row runs only on
the author's machines; the others run anywhere.

### 14.5 Property tests

Random trees (generated modules with random include graphs and `#if` gates):
the closure of 9.8 must equal the transitive closure computed by a slow
reference; output is deterministic; every emitted rule's prerequisites exist.

---

## 15. Roadmap: from today's code to this specification

Order by value to the author's use case; each step ends in a state where the
tool is still usable and tests pass.

| step | content | closes |
| --- | --- | --- |
| **0** | fix scan blockers: `cpp-scan-stray-endif`; scan diagnostics policy (5.10); `--explain` skeleton | BUGS |
| **1** | **project file + `--dump-project`** (INI, 11.2) and origin tracking. Pure addition; command line keeps working | 11 |
| **2** | **symbol closure inference** for the flat layout (9.8) behind `--infer=closure`; compare against `pictest` in the conformance harness (14.4 rows 1 to 4) | the author's main wish |
| **3** | **XC8 and SDCC descriptors** as C tables first (the facts of TASKS 4/5), then their conversion to data (step 5) | TASKS 4, 5, 7 |
| **4** | **MPLAB X importer** (9.10), one `conf` per point | usb-stack, usb-uc |
| **5** | descriptor interpreter: move `compiler.c` tables to `.tc` files; add gcc/clang/msvc probing | 6, 5.2 |
| **6** | IR + capability table (7.1) replacing per-make-type `if`s in `output.c`; `-MMD` depfiles | 7 |
| **7** | requirements system and omission report (5.6); library hints (5.8) | HANDOFF 1 to 3 |
| **8** | matrix driver (8.2), board profiles, constraints (8.5, 9.12) | TASKS 3 (extension) |
| **9** | cache and parallel scan (13.2, 13.3) | speed |
| **10** | install/dist/check/size targets (10), probes (9.5), packages (9.4) | production |
| **11** | other toolchains (AVR, ARM, XC16/32, bcc56, z88dk) as descriptors only | breadth |

Each step adds: tests (14), ChangeLog entry, `doc/genmakefile.md` update, and
removes the corresponding `TODO` entry. Steps 1 to 4 are the minimum for "infer
build parameters from the sources there and produce well built firmware"; the
author's five repositories then build from `genmakefile` alone.

### 15.1 Open questions for the author

1. ~~Is HI-TECH C still a target?~~ **Answered 2026-11-01: dropped** (discontinued, now
   XC8). Removed from the code; XC8 v1.x stays. `seg7test_CCVER = 9.8x` in pictest
   is therefore unsupported and is reported as `unsupported toolchain` by the importer.
2. Should the MPLAB X importer also *write* MPLAB X projects (`mplab.c` does
   today, partially) from the same description, so one source of truth feeds
   both the IDE and make?
3. Which flasher commands to model first (`pk3cmd`, `ipecmd`, `pk2cmd`)?
4. Is Windows-only toolchain use under wine for Proteus debug builds (the
   `glcdtest-*.mk` case in pictest's `CLAUDE.md`) a standing requirement? It needs
   the `runner`/`system=mingw` combination of 9.6 to produce a `.cof` with full
   debug symbols.

## Implementation status

What of this specification is in the program (see also `TODO`,
`genmakefile-implement-spec`):

* compiler types `xc8` and `sdcc`, `--chip`, `--xtal`, `--baud`, the build id in
  the output directories;
* `--pair-headers`: a quoted `#include "x.h"` adds `x.c` (section 9, step 1 of
  the source inference, by header name only);
* `--import-mplabx FILE`: chip, `-D`, `-I` and sources of the first
  configuration of a MPLAB X project (section 9.12, one configuration);
* checked against lc-meter + libpicp, USB-uC and the USB-Stack HID mouse
  example; the scripts are `tests/genmakefile/pair-headers.sh` and
  `tests/genmakefile/import-mplabx.sh`.

`--import-mplabx` also reads MPLAB 8 `.mcp` files (device and file list).

`--mplabx-conf NAME` selects one configuration. `an-tronics` (a single C program) builds with
`-t gcc` (checked with make).

`--project FILE` reads the `[project]` section of 11.2 (keys `toolchain make chip xtal baud
build define include library link import conf pair-headers sources`) and turns it into
command line arguments, in front of the real ones.

Not yet: the other sections of the project file (`[module]`, `[program]`, `[switch]`, `[matrix]`, ...), the origin of each value in `--dump-project` (it prints the merged arguments now), the compiler options of a `.mcp`, all configurations of a
MPLAB X project as a build matrix in one run, a symbol closure that sees variables and macros (`--infer-sources` follows the called functions).

