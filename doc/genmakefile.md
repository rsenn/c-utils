# genmakefile

`genmakefile` writes a build description for C sources: a makefile for
GNU make, NMake, Borland make or another `make`, a Ninja file, a Bourne
shell or Windows batch script, an MPLAB project, or a CMake file. It is
meant for compilers and platforms where running CMake is not an option
(Borland C++ 5.5, lcc, Pelles C, SDCC, XC8, ...), and for projects that
follow the layout of this repository.

It does not compile anything and needs no configuration step. It looks at
the sources, finds out what is a program and what is a library, which
header each file depends on, and writes the rules.

    genmakefile [options] [sources or directories...] [-- [options] ...]

The sources and directories can be given as arguments, read from a build
log (`-f`) or taken from a CMake build directory (`-M`).

## Quick start

A GNU makefile for every program below `src/dnsutils`:

    genmakefile -m gmake -t gcc -o gcc.mk src/dnsutils
    make -f gcc.mk

A makefile for Borland C++ 5.5 for every program below `src/`, built with
Borland make under wine:

    genmakefile -m borland -t bcc55 -I/opt/bcc55/include -o bcc55.mk src/
    WINEPATH='Z:\opt\bcc55\bin' wine /opt/bcc55/bin/make.exe -f bcc55.mk

`WINEPATH` has to name the directory of `ilink32.exe` and `tlib.exe`.
Add `-i` to the `make.exe` call to go on after a failing compile.

Run `genmakefile` from the top of the source tree: the paths in the
generated file are relative to the working directory, and so is the
directory of the lib modules (`lib`).

## What it finds

- **Programs.** A `.c` file that defines `main()` is a program. It is
  linked to the objects of the same directory it names (`foo.h` in the
  program selects `foo.c` next to it, when that has no `main()`), and to
  the library modules it uses.
- **Library modules.** `lib/<name>.h` together with a directory
  `lib/<name>/` of sources is a module. It is archived into
  `<name>.a` (`<name>.lib` with Borland, Microsoft and lcc compilers). A
  program links the modules of the headers it includes, and those modules
  link what they need in turn. The directory is `lib` unless `--lib-dir`
  says otherwise.
- **Dependencies.** Every object depends on its source and on every header
  the source includes, directly or not.

How the headers are found depends on the compiler:

- For a compiler whose predefined macros are known (`bcc55` at the moment)
  each source is run through the preprocessor of `lib/cpp` with those
  macros. `#if` and `#ifdef` gates are evaluated for the target, so a
  header that is only included on Windows is a dependency only when the
  target is Windows. Headers below the `-I` directories that lie outside
  the working directory (the compiler's own headers) are not read, and a
  header that cannot be found is skipped.
- For every other compiler the sources are read as text and each
  `#include "..."` line counts, whatever gate it sits in.

## Options

### Input and output

| option | meaning |
|---|---|
| `-o, --output FILE` | write to `FILE` instead of standard output |
| `-f, --input-file FILE` | read the compile and link commands from a build log |
| `-M, --cmake-builddir DIR` | read a CMake build directory (`link.txt`, `compile_commands.json`) recursively |
| `-n, --name NAME` | name of the output program (when there is only one) |
| `-W, --width N` | wrap the generated lines at `N` columns |

### What to generate

| option | meaning |
|---|---|
| `--create-bins` | rules for programs |
| `--create-libs` | rules for libraries |
| `--create-objs` | rules for objects |
| `--create-module` | rules for a loadable module |
| `--no-create-...` | switch one of these off |
| `-S, --build-as-lib TARGET` | build `TARGET` as a library |
| `-i, --install` | add install rules |
| `--lib-dir DIR` | directory of the lib modules (default `lib`) |

Without any of the `--create-` options programs, libraries and objects are
all generated.

### Directories and names

| option | meaning |
|---|---|
| `-O, --outdir DIR` | output directory |
| `-d, --builddir DIR` | build directory |
| `-k, --bindir DIR` | directory of the linked programs (default: the build directory) |
| `-w, --workdir DIR` | prefix written in front of the target names and in `VPATH` |
| `-T, --objext EXT` | object file extension |
| `-B, --exeext EXT` | program file extension |
| `-X, --libext EXT` | library file extension |

When no build directory is given it is named after the compiler and the
build type, for example `bcc55-Release` or `gcc-Debug`. Objects of the
Borland scan mirror the source tree below it
(`bcc55-Release\src\dnsutils\dnsip.obj`), libraries go to
`bcc55-Release\lib\<name>.lib`, and programs are written to
the binary directory (`-k`, by default the build directory itself).

### Compiler and target

| option | meaning |
|---|---|
| `-t, --compiler-type TYPE` | compiler: `gcc`, `clang`, `tcc`, `msvc`, `icl`, `bcc55`, `bcc`, `lcc`, `occ`, `dmc`, `pocc`, `zapcc`, `htc`, `xc8`, `sdcc`; guessed from `-o` or `-d` when absent |
| `-m, --make-type TYPE` | program that runs the file: `gmake`, `make`, `nmake`, `borland`, `omake`, `pomake`, `ninja`, `shell`, `batch`, `mplab`, `mplabx`, `cmake`; derived from the compiler when absent |
| `-a, --arch MACHINE` | target machine, for example `x86_64` or `i386` |
| `-s, --system OS` | target operating system |
| `-c, --cross TARGET` | cross compiler prefix |
| `-p, --chip CHIP` | microcontroller (PIC compilers) |
| `-P, --preprocessor CMD` | also write preprocess rules using `CMD` |
| `--debug` | debugging information, no optimization |
| `--relwithdebinfo` | debugging information, optimize for speed |
| `--release` | optimize for speed (the default) |
| `--minsizerel` | optimize for size |
| `--lang-c`, `--cxx` | compile as C or as C++ |

### Flags and libraries

| option | meaning |
|---|---|
| `-D NAME[=VALUE]` | add a preprocessor definition, also while scanning |
| `-I DIR` | add an include directory |
| `-L DIR` | add a library directory |
| `-l LIB` | link a library |

## Several targets in one file

Segments separated by a literal `--` are separate targets. Options before
the first `--` are common; each later segment has its own options and
sources, and all rules go into the same output file:

    genmakefile -m gmake -t gcc -o all.mk \
        --create-libs lib -- --create-bins src/dnsutils -- --create-bins src/ftp

## Reading existing build output

`-f` reads a log of a build (compile and link command lines, as printed by
`make` or `ninja -v`) and writes it back as rules for the chosen make type.
`-M` reads a CMake build directory: every `link.txt` and
`compile_commands.json` below it. This is how a makefile for a different
compiler is made from the commands CMake would run, but without the
defines, flags and libraries that differ on the other platform, which have
to be given as options.

## Borland C++ 5.5

The Borland make file written for `-m borland -t bcc55` uses `.obj`,
`.exe` and `.lib`, `tlib` for libraries and `bcc32` for linking with
`cw32mt.lib` and `import32.lib`. Libraries and the link line use inline
files (`@&&|`).

Borland make has `!if`, `!elif`, `!else`, `!endif`, `!ifdef`, `!ifndef`,
`!include` and `-DNAME=value` on its command line, but it does not expand
a macro name made of another macro (`$(CFLAGS_$(BUILD))` is empty). The
generated file therefore has one build type; choose it with `--debug`,
`--release`, `--relwithdebinfo` or `--minsizerel` when generating.

Under wine the output directories are made by the first rules of `all`,
so build `all` (the default target) rather than a single object or
library.

## Known limits

- The preprocessor scan exists for `bcc55` only. Other compilers use the
  text scanner, which does not evaluate `#if`.
- A source file that does not end in a newline after a final `#endif` is
  reported by the preprocessor and gets no header dependencies (the entry
  `cpp-scan-stray-endif` in `BUGS`).
- The sources of this repository do not compile with bcc32 yet; the
  generated makefile is correct, the compiler stops on the sources.
- Programs are placed in one directory by their base name, so two programs
  of the same name in different source directories collide.
