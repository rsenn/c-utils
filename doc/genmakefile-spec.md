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

