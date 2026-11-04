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
| `-v`, `--verbose` | tell what is being done; twice: also every source the scan starts with |
| `-G`, `--dump` | dump the state (only in a build with `DEBUG_OUTPUT`); twice: also every rule |
| `-x FILE` | write the debug log to FILE |
| `--dep NAME[,NAME]` | optional packages: the make file enables `HAVE_<NAME>` when `LIB<NAME>` is set (see Optional packages) |
| `--switch-prefix STR` | names the sources query that begin with STR are switches (default `USE_`) |
| `--xtal HZ` | clock of the microcontroller, defines `_XTAL_FREQ` |
| `--baud BPS` | baud rate of the serial port, defines `UART_BAUD` |
| `--code-offset ADDRESS` | address the program is loaded to |
| `--cflag FLAG`, `--ldflag FLAG` | xc8-cc: a flag for compile and link, or for the link only (repeatable); `--import-mplabx` makes them from the HI-TECH-COMP, HI-TECH-LINK and XC8-config-global properties |
| `--pack VENDOR/NAME/VERSION` | xc8-cc: device family pack; the makefile gets `DFP_DIR ?=` its directory when found (else a warning) |
| `--toolchain-version V` | xc8-cc: compiler version; the makefile gets `XC8_BIN ?=` and `export PATH` when `v<V>/bin/xc8-cc` is found |
| `--universal-hex`, `--no-universal-hex` | PIC18 with a code offset: also write `<name>-universal.hex` (default on), see below |
| `--program NAME` | name of the image (`NAME.elf`, and for xc8-cc `NAME.hex`) |

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
| `-t, --compiler-type TYPE` | compiler: `gcc`, `clang`, `tcc`, `msvc`, `icl`, `bcc55`, `bcc`, `lcc`, `occ`, `dmc`, `pocc`, `zapcc`, `xc8`, `sdcc`; guessed from `-o` or `-d` when absent |
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

## Switches found in the sources

The preprocessor scan notes every name a conditional asks about while no
macro of that name exists: with `#ifdef`, `#ifndef` or `defined()`, or used
by value in an `#if` or `#elif`. Branches that are not taken are read for
this too. A name is a switch of the build when

- it begins with `USE_` (change it with `--switch-prefix`, an empty prefix
  takes every name),
- no `#define` of the sources gives it a value, the fallback
  `#ifndef USE_X` / `#define USE_X 1` / `#endif` being one, and
- no `#if` compares it or calculates with it (`#if USE_X > 1`); only a plain
  truth value, as in `#if USE_X` or `#if !USE_X && USE_Y`, qualifies.

For each switch the generated file has a definition that is added to `DEFS`
when the build is given a value:

    make USE_UART=1              (GNU make and others)
    make -DUSE_UART=1            (Borland make)
    nmake USE_UART=1             (nmake)

Without a value the name stays undefined, so the file builds the same as
before. `-v -v` lists the names the scan found and tells why a `USE_` name
is no switch. Only the preprocessor scan finds switches, which is used for
the compilers whose macros are known (bcc55 so far).

## Firmware builds (one file per point of the matrix)

The firmware of a microcontroller is built for a compiler, a chip, a build
type, a clock, a baud rate and a load address. A file holds one such point;
a shell loop makes the others. With `--chip` and at least one of `--xtal`,
`--baud` and `--code-offset`, the layout is

    obj/<compiler>-<chip>/<build id>/<program>/<source>.<ext>
    bin/<compiler>-<chip>/<program>_<build id>.<ext>

with the build id `<type>_<mhz>mhz_<kbps>kbps[_at<offset>]`, for example
`debug_20mhz_38kbps` or `release_intosc48mhz_38kbps_at0x2000`. 48 MHz is the
internal oscillator with its PLL and reads `intosc48`; an offset of zero is
left out. Every program has objects of its own, since a program may be given
other macros than the next one.

    genmakefile -t xc8 -m make -I. -Ilib -Isrc --chip=18f25k50 \
        --xtal 48000000 --baud 38400 --code-offset 0x2000 --release \
        -o rgbtest-18f25k50-xc8-release.mk src/ lib/

## One hex for both: with and without the bootloader

An application linked at a code offset (`--code-offset 0x2000`, or
`additional-options-code-offset` of a MPLAB X project) leaves 0x0000..0x1FFF
empty. The USB-uC bootloader skips every hex block below its
`PROG_REGION_START` (0x2000) and ignores the ID and configuration records; its
own vectors forward 0x08 to 0x2008, 0x18 to 0x2018 and the reset to GOTO 0x2000.
Without the bootloader (a PICkit) nothing sits at the vectors: the reset only
works by sliding over NOPs, and the interrupt vector 0x0008 slides into 0x2000,
which restarts the program.

For PIC18 with a code offset genmakefile therefore writes a second image,
`<name>-universal.hex`, made from the plain `<name>.hex` (which stays as the
compiler wrote it):

    $(IHEXTOOL) vectors -a $(CODE_OFFSET) -o $@ $<

`ihextool vectors` adds a `GOTO (vector + offset)` at 0x00, 0x08 and 0x18
(`--vectors` changes the list; the encoding is bytes `k&ff EF (k>>8)&ff F0|(k>>16)&0f`
with k = target/2) and replaces data that is already there, with a warning.
The bootloader ignores the stubs, a programmer loads them. `IHEXTOOL` and
`CODE_OFFSET` can be overridden on the make command line. The rule is on by
default for PIC18 when the offset is not zero (gmake and make, xc8 and xc8-cc;
not ninja or the script backends); `--no-universal-hex` turns it off. PIC16 (14
bit core) uses other encodings and is not supported yet.

## Optional packages

The compression packages the sources query with `#if HAVE_BROTLI`,
`HAVE_LIBBZ2`, `HAVE_LIBLZMA` and `HAVE_ZLIB` are found by the scan, and
`--dep NAME` adds any other package (`HAVE_<NAME>`, enabled by `LIB<NAME>`).
The generated file leaves them off until the make variable is given, either
on the command line or in the environment:

| Variable | Effect |
|---|---|
| `LIBBROTLI`, `LIBBZ2`, `LIBLZMA`, `LIBZ` | the library names; defines `HAVE_<..>=1` and adds them to `LIBS` |
| `<VAR>_INCLUDEDIR` | adds `-I<dir>` to `CPPFLAGS` |
| `<VAR>_LIBDIR` | adds the library path to `LDFLAGS` |

Borland make: `make -DLIBZ=zlib.lib -DLIBZ_INCLUDEDIR=C:\z\include -DLIBZ_LIBDIR=C:\z\lib`.

## Flat libraries (third party trees)

`--library NAME` turns every source without a `main()` among the arguments
into the library `NAME` (`NAME.lib` in the library directory of the output),
and the programs link it. With `-i` an `install` rule copies the library to
`$(prefix)/lib` and the public headers to `$(prefix)/include`: the headers
below an `include` directory of the sources (keeping the part after it) and
those named with `--header`. `prefix` defaults to `inst-<compiler>`.

    genmakefile -m borland -t bcc55 -I/opt/bcc55/include -L/opt/bcc55/lib \
      --library bz2 --header bzlib.h --create-libs --no-create-bins -i \
      -o bcc55.mk .
    make -f bcc55.mk install

The result is what a project passes back with `LIBBZ2=bz2.lib`,
`LIBBZ2_INCLUDEDIR=...\include` and `LIBBZ2_LIBDIR=...\lib` (see Optional
packages).

## Embarcadero C++ (bcc102)

`-t bcc102` generates for `bcc32c`, the clang based C99 compiler of
Embarcadero C++ 10.2. Its configuration file supplies the include and
library paths of the compiler; give `-L<dir>/lib/win32c/release` (and
`.../psdk`) for the start-up object and the run time, which the make file
passes to `ilink32`. The linker is `$(ILINK)`: under wine the linker of the
same package stops with LME200, and the one of bcc55 links the objects, so
there use `make -DILINK=/opt/bcc55/bin/ilink32.exe`.

