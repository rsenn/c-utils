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
