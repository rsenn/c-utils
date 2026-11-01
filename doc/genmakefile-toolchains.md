# Toolchain knowledge extracted from the author's build systems

Source of everything below: the build systems the author already has for the
PIC repositories (cloned 2026-11-01), read line by line:

| origin | files |
| --- | --- |
| hand written make | `pictest/build/{vars,xc8,sdcc,htc,mcc18,common,rates,targets}.mk`, top level `Makefile`, `gen-mk.sh`; `lc-meter/build/*.mk` (same family) |
| scripts | `pictest/scripts/xc8.sh`, `xc8-cpp.config`, `baud.sh`, `find-pic-header.sh`, `ipecmd.sh`, `pk3cmd.sh` |
| genmakefile output | `libpicp/Makefile.{xc8,sdcc,htc}`, `htc.ninja`, `gen-ninja-build.sh`, `pictest/glcdtest-*.mk` |
| MPLAB 8 | `build/mplab/*.mcp` (option ids per tool GUID) |
| MPLAB X | `*.X/nbproject/{configurations.xml,Makefile-<conf>.mk,Makefile-local-<conf>.mk}` of `usb-stack` (8 projects, 28 confs) and `usb-uc` (6 projects) |
| XC8 argument files | `lc-meter/LC-meter-{debug,release}.xc8` |
| existing genmakefile knowledge | `src/genmakefile/compiler.c` (sdcc, xc8 v1, xc8-cc v2+), `mplab.c` (MPLAB 8 option ids), `mplab-nbproject.txt` (MPLAB X XML template) |

The purpose is to turn this into the *data* of the toolchain descriptors
(`toolchains/*.tc`, see `genmakefile-spec.md` section 6; drafts are in
`src/genmakefile/toolchains/`). Each fact names where it was seen.

## Decision: HI-TECH C is dropped

HI-TECH C (`picc`, `picc18`, versions 9.x) is a discontinued product that is now
XC8. The `htc` compiler type was removed from genmakefile (2026-11-01).
**XC8 v1.x stays**: it is the same flag family (`--pass1`, `--chip=`, `--opt=`,
`--runtime=`), is what the author builds with (`CCVER ?= v1.43`, `v1.45`), and
is what the `.xc8` argument files and half of the MPLAB X projects use.
Everything in the htc sections of the source files below is kept here only
where it is also true for XC8 v1.

All other compilers stay and are to be *improved*: the spec makes no compiler
removable by an implementation convenience (decision of the author).

## 1. Compiler families seen

| family | driver | objects | outputs | seen in |
| --- | --- | --- | --- | --- |
| **XC8 v1.x** (`xc8`, HI-TECH heritage) | `xc8` (and `--pass1` for compile-only) | `.p1` | `.hex .cof .elf .map .bin .cfg(piccfg) .as(.asmlist) .lst` | pictest/lc-meter `xc8.mk`, MPLAB X v1.43/1.45 confs, `.xc8` files |
| **XC8 v2.x+** (`xc8-cc`, LLVM front end, gcc-like CLI) | `xc8-cc`, `xc8-ar` | `.p1` (even with `-o foo.o`!) | `.hex .elf .cof(debug) .map memoryfile.xml` | MPLAB X v2.31 (97 confs) and v2.46; `compiler.c` branch `xc8-cc` |
| **SDCC** (3.6.0 default, 4.6.0 seen) | `sdcc`, `sdar`, `gplink`/`gpasm` | `.o`/`.rel` | `.hex .cod .lst .asm` | `sdcc.mk`, `libpicp/Makefile.sdcc` |
| **MCC18** (legacy, PIC18 only) | `mcc18`, `mplink`, `mplib` | `.o` | `.hex .cof .map .sym .lst` | `build/mcc18.mk` |
| **gputils** (MPASM source) | `gpasm`, `gplink` | `.o` | `.hex .cod .lst` | `usb-uc/Assembly Versions/…` (`${MP_AS} -p16f1454`) |
| ~~HI-TECH C~~ | `picc`, `picc18` | `.p1` | | dropped |

## 2. XC8 v1.x

### 2.1 Flags (compile pass 1)

| flag | meaning | seen with |
| --- | --- | --- |
| `--pass1` | compile only, writes `<name>.p1` into `--outdir` (**default: the current directory**) | xc8.mk, MPLAB X |
| `--chip=<chip>` | device, lowercase works (`18f25k50`); `--chipinfo` lists valid names | all |
| `-q` | quiet banner | xc8.mk |
| `--mode=pro` / `PRO` | operation mode (case insensitive); free mode has the weaker optimiser | all |
| `--opt=default,+asm,+asmfile,-speed,+space,-debug,-local[,N]` | optimisation switches; the trailing integer is the level 1 to 9 | xc8.mk (`+speed,-space,9`), MPLAB X (`-speed,+space`) |
| `-G` | debug info for the debugger (`.cof` symbols) | debug builds |
| `--debugger=icd2\|pickit3\|none` | tells the compiler which debugger reserves resources | xc8.mk `icd2`, MPLAB X `none`, pictest `pickit3` |
| `--double=24\|32 --float=24\|32` | floating point sizes (**24 saves flash; 32 is IEEE**) | `--float=24 --double=32` (pictest), `--double=24 --float=24` (usb) |
| `--warn=N` | warning level, `-9`..`9`; `--warn=9` all | 3 and 9 |
| `-N<n>` | max identifier length | `-N127`, `-N255` |
| `--asmlist` | write `.lst` of the generated assembly | all |
| `-P` | preprocess assembler files too | MPLAB X |
| `--addrqual=ignore\|require\|reject` | `near/far/__near` qualifier policy | `ignore` |
| `--emi=wordwrite\|byte…` | external memory interface write mode (PIC18) | `wordwrite` |
| `--errformat= --warnformat= --msgformat=` | diagnostic message format; `%f:%l: warning: (%n) %s` makes them gcc-like for editors | xc8.mk, MPLAB X |
| `@file` | read options from a file, one per line; **values with spaces are quoted** | `scripts/xc8.sh`, `.xc8` |
| `-D<name>=<v>` `-I<dir>` `-U<name>` | preprocessor | all |
| `--scandep` | write dependency list (a `.dep` file, Windows paths with backslashes, a colon after drive letters) | HI-TECH `htc.mk` rule; `sed` removes `:` lines and converts `\` |
| `-S` | stop after assembly (`.as`) | xc8.mk `$(ASSRCS)` |
| `--pre` | preprocess only | `xc8-help.txt` |
| `-m<file>` | link map file | `-m$(BUILDDIR)$(PROGRAM)_$(BUILD_ID).map` |
| `-o<file>` | output, **no space** | all |

### 2.2 Flags (link)

| flag | meaning | seen with |
| --- | --- | --- |
| `--output=-mcof,+elf` | choose output formats: `+/-` before names (`mcof`=Microchip COF, `elf`, `inhx032`=Intel hex with 32 bit addresses, `bin`, `default`) | `--output=default,-inhx032`, `--output=+mcof,-elf:multilocs` |
| `--summary=default,-psect,-class,+mem,-hex,-file` | what the link summary shows | |
| `--runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,+download,+clib` | startup code options: `clear` (BSS), `init` (data), `keep` (startup.as), `no_startup`, `osccal` (restore factory oscillator calibration), `resetbits` (save STATUS bits), `download` (format hex for bootloader), `config` (default config words), `clib`, `plib` | pictest vs MPLAB X, differing by project |
| `--stack=compiled[:auto:auto:auto]` | stack model; **the number of colon fields depends on the family**: PIC16 `compiled:auto:auto`, PIC18 `compiled:auto:auto:auto` | MPLAB X |
| `--codeoffset=<hex>` | load address (bootloader app); **only at link** | xc8.mk `LDFLAGS += --codeoffset=$(CODE_OFFSET)` when non zero |
| `--OUTPUT=lpp` | build a library (`.lpp`) instead of a program; library tool is the same driver | `compiler.c` |
| `--memorysummary` | memory report file | `compiler.c` |
| `--chip=` `-G` `--opt=…` | **the link command also takes the compile flags** (chip, optimisation, debug) — MPLAB X passes the whole set at link | xc8.mk `$(LD) $(LDFLAGS) $(COMMON_FLAGS)` |

### 2.3 Quirks

1. **Compile from inside the object directory.** XC8 v1 writes some files
   (`funclist`, `.dep`, `startup.as`) to the current directory, so pictest
   runs `cd obj; xc8 --pass1 … -I../lib … --outdir=<subdir> ../src/x.c`: every
   `-I<dir>` becomes `-I../<dir>`, the source gets `../`, `--outdir` is relative
   to `obj/`. (xc8.mk: `$(CPPFLAGS:-I%=-I../%)`, `--outdir=$(OBJDIR:obj/%/=%)`.)
   Descriptor: `compile-cwd = objdir` with path rewriting.
2. **Redefinition of a macro on the command line is fatal** (`-DUART_BAUD`
   twice, vars.mk comment on `miditest`). Defines must be a set keyed by name:
   a later `-DX=…` replaces an earlier one, and `-DX` twice with different
   values is reported by genmakefile, never emitted.
3. **`__at()` takes only constants in v1.43 PRO** (an arithmetic expression is
   rejected), so address defines are precomputed and passed as `-D`
   (`EP0_OUT_EVEN_BUFFER_BASE_ADDR=0x41C` …). A switch in the project file may
   carry `define =` lists; a *derived constant* feature (expression evaluator
   in the project file) is a roadmap item (spec 9.5).
4. **hex format for 18F:** `--output=default,-inhx032` selects INHX32 vs
   INHX032; PIC16 uses `-ginhx32` under v2. Wrong choice yields a hex the
   programmer rejects.
5. **`.cof` and `.elf` need `--output=-mcof,+elf` together with `-G`** for
   debug symbols in Proteus/MPLAB; this is the `glcdtest-*.mk` case.
6. **The `.cfg` file** (config bits report) is made by a separate tool,
   `piccfg $(HEXFILE) > $(CFGFILE)` (common.mk), not by the compiler.
7. **Identical option vocabularies for debug/release differ in more than `-G`:**
   debug: `--opt=default,+asm,+asmfile,-speed,-space,+debug`, `-D__DEBUG=1 -D_DEBUG=1`;
   release: `--opt=default,+asm,+asmfile,+speed,-space,-debug,9`,
   `-DNDEBUG=1 -D__NDEBUG=1`; minsizerel: `…-asmfile,-speed,+space,+debug,3`
   (`compiler.c` and xc8.mk agree).
8. **A separate preprocessor in front of the compiler.** `scripts/xc8.sh`
   runs a Boost.Wave based `cpp` (`wave-xc8`, `cpp-xc8.exe`, repository
   `wave-cpp-xc8`) first, with the XC8 predefs (`-D__XC8__ -D__PICCPRO__
   -D__PICC__ -D__XC__ -p1 --c++11 --disambiguate=0 -S <INCDIR> -F pic.h`
   from `xc8-cpp.config`), and then feeds the preprocessed text to `xc8`. The
   reason is not documented in the repository (probably the compiler's own
   preprocessor, for `__at()` or macro expansion limits). The consequence for
   genmakefile: `xc8` is a toolchain with `preprocess-first = yes` and a
   configurable preprocessor command (`cpp = …`); this is also the exact
   predefs list for the `lib/cpp` scan (spec 5.2): `__XC8__ __XC__ __PICC__
   __PICCPRO__ HI_TECH_C` plus the chip macros below.
9. **Predefs (known, no probe possible without a licence):**
   `__XC=1 __XC8=1 __XC__=1 __XC8__=1 __PICC__=1 __PICCPRO__=1 HI_TECH_C=1
   _XTAL_FREQ=<hz> __<chip>=1 PIC<CHIP>=1 PIC16=1|PIC18=1` (from `compiler.c`,
   pictest `vars.mk`, `xc8-cpp.config`).
10. **Library file extension is `.lpp`, archiver is the compiler driver itself**
    (`$(CC) … --OUTPUT=lpp …`), old variants use `mplib`.
11. **Install locations:** Linux `/opt/microchip/xc8/<ver>/bin/xc8` (`v1.43`
    with the `v`); Windows `C:/Program Files[ (x86)]/Microchip/xc8/<ver>`;
    `MCHP_XC8=1` is defined by the author, not the compiler.
12. **The compiler version is per chip/program:** `v2.36` for `18f16q41`
    (family not supported by v1); `CCVER ?= v1.43` otherwise.

### 2.4 Chip helper facts

* chip name case: `chipl` for flags (`--chip=18f25k50`), `chipu` for macros
  (`PIC18F25K50=1`); both forms appear in `-D`.
* family from the name: `10f|12f|16f` mid-range (`PIC16`), `18f` PIC18
  (`PIC18`); `12f1840`/`16f1xxx` are *enhanced* mid-range; `18f16q41` is
  v2-only. The first-three-letter test `head -c 3` in sdcc.mk is the family
  key; descriptor `[chip.family]` replaces it.
* baud sanity: `scripts/baud.sh` computes `SPBRG = Fosc/(M*baud) - 1` with
  `M = 64 (BRGH=0, async), 16 (BRGH=1), 4 (sync)` and the resulting error; the
  matrix check in spec 8.5 reuses this formula.

## 3. XC8 v2.x and later (`xc8-cc`)

Seen as 97 + 4 MPLAB X confs. The vocabulary is gcc-like, **not** v1.

| v1 | v2 | notes |
| --- | --- | --- |
| `--chip=X` | `-mcpu=X` | `-mcpu=$(MP_PROCESSOR_OPTION)` |
| `--pass1 … -o x.p1` | `-c -o x.p1` | `.p1` always (`-o foo.o` still writes `foo.p1`; `compiler.c` notes this) |
| `--mode=pro` | (none; optimiser by licence) | |
| `--opt=…,+speed,-space` | `-Os` / `-O1..3` / `-Og` | `-fasmfile` is `+asmfile` |
| `-G`, `--debugger=none` | `-gdwarf-3`, `-g` | |
| `--double=24 --float=24` | `-fno-short-double -fno-short-float` (**the default is the opposite: 24 bit**; the flags request 32) | |
| `--warn=-3` | `-mwarn=-3` | |
| `--asmlist` | `-Wa,-a` | |
| `-P` | `-xassembler-with-cpp` | |
| `--emi=wordwrite` | `-memi=wordwrite` (PIC18 only) | |
| `--addrqual=ignore` | `-maddrqual=ignore` | |
| `--summary=…` | `-msummary=…` | |
| `--codeoffset=0x2000` | `-mcodeoffset=0x2000` | link |
| `--output=default,-inhx032` | `-ginhx032` (PIC18) / `-ginhx32` (PIC16) | |
| `--runtime=…,-keep,-no_startup,+download…` | `-mno-keep-startup -mno-download -mdefault-config-bits -mno-osccal -mno-resetbits -mno-save-resetbits -mno-stackcall` (PIC16 only for the last four) | per family |
| `--stack=compiled:auto:auto:auto` | `-mstack=compiled:auto:auto[:auto]` | 2 or 3 `auto` by family |
| `--errformat=…` | (message format fixed) | |
| (none) | `-std=c99` | |
| (none) | `-mdfp=<dir>/xc8` | **device family pack path**: `~/.mchp_packs/Microchip/<PACK>/<ver>` (`DFP_DIR`); required for v2 |
| (none) | `-Wl,-Map=<f>`, `-Wl,--memorysummary,<xml>`, `-Wl,--data-init`, `-Wl,--defsym=__MPLAB_BUILD=1` | link |

Quirks:

1. **Compile and link use the same flag list** (MPLAB X passes everything to
   both); `-c` selects compile only.
2. `-mdfp` is not optional: the packs live outside the compiler
   (`PIC18F-K_DFP`, `PIC18Fxxxx_DFP`, `PIC16Fxxx_DFP`); genmakefile must find
   or be told them (`DFP_DIR`), and for **ninja** the variable must reach the
   shell unexpanded (`$$DFP_DIR`) — this is already handled in `compiler.c`.
3. MPLAB X also defines `-DXPRJ_<conf>=<conf>`; harmless but present in IDE
   builds; the importer drops it unless asked.
4. Dependency output: MPLAB X renames the compiler's `<obj>.d` to `<obj>.p1.d`
   and normalises it with `fixDeps` (v1 and v2 makefiles both show
   `${MV} …d …p1.d` and `${FIXDEPS}`); v2 is gcc-like, so `-MMD` style is
   expected but not verified here.

## 4. SDCC (pic14, pic16)

### 4.1 Flags

| flag | meaning | seen |
| --- | --- | --- |
| `-mpic14` / `-mpic16` | port: 12F/16F vs 18F. **`12f1840` needs `-mpic14` forced** even though the chip test would not say so | sdcc.mk |
| `-p<chip>` | device (`-p18f25k50`) | |
| `--use-non-free` | required for the device headers/libraries of the non-free gputils tree | |
| `--opt-code-speed` / `--opt-code-size` | optimiser target | sdcc.mk `OPT=speed\|space` |
| `--float-reent` | reentrant float lib | libpicp |
| `--debug` | emits `.cod`/`.adb` debug info; **given at compile and link** | sdcc.mk |
| `-c` `-S` `-E` `-o` | object, assembly, preprocess, output | |
| `-D__<chip>=1 -DSDCC=1 -D__SDCC=1` | macros | compiler.c, sdcc.mk |
| `-llibm.lib` / `-llibm18f.lib` | **math library by port**: pic14 `libm.lib`, pic16 `libm18f.lib` | sdcc.mk |
| `-Wl-s<script>` / `-Wl<script>` | pass linker script options to gplink | code offset |
| `--ivt-loc=<addr>` | interrupt vector location | code offset + 8 |
| `--out-fmt-ihx` | produce `.ihx` instead of `.hex` | libpicp |
| `--code-loc` | **no-op for the pic16 port** | sdcc.mk comment |

### 4.2 Bootloader (code offset) recipe

`sdcc.mk` documents exactly what is needed for an application that is
loaded behind a bootloader:

1. a *custom linker script*: copy gputils' default
   `$(GPUTILS_PREFIX)/share/gputils/lkr/<chip>_g.lkr` and change the line
   `CODEPAGE NAME=page START=0x0` to `START=<offset>`
   (`sed 's/CODEPAGE   NAME=page       START=0x0 /CODEPAGE   NAME=page       START=$(CODE_OFFSET) /'`).
   `GPUTILS_PREFIX` is found from `which gplink` (the script directory lies
   next to the binary sdcc really uses);
2. `-Wl-s -Wl<generated.lkr>`;
3. `--ivt-loc=$(( CODE_OFFSET + 8 ))` — the vector trampoline lands at offset+8,
   which is where the bootloader (`USB-uC/USB_uC.X`) jumps;
4. **post-process the hex**: sdcc's crt0 always writes a reset-vector `GOTO`
   at true 0x0000 regardless of the shifted code page, which would overwrite
   the bootloader's own vector table. An `awk` pass on the Intel hex removes
   data records below the offset (record type 04 gives the upper address,
   type 00 the data; drop records whose address < offset):
   `awk -v off=$((OFFSET)) 'BEGIN{u=0} {t=substr($0,8,2)} t=="04"{u=strtonum("0x" substr($0,10,4)); print; next} t=="00"{if(u*65536+strtonum("0x" substr($0,4,4))<off) next; print; next} {print}'`.
   (XC8's `--codeoffset` avoids emitting the stub at all.)

Descriptor form: `[rule.lkr]`, `[rule.hexfilter]` (spec 6.4). The awk filter is
the portable part; a `hexstrip` helper in `lib/ihex` (the repository has one)
would replace awk and `strtonum` (a GNU awk extension) on hosts without gawk.

### 4.3 Quirks

1. `gplink` always writes a `.cod` symbol file next to the `.hex`; make
   must know it as an output (`CODFILE`) or `clean` misses it.
2. Archiver is `sdar`, `r` for create/replace (`$(LIB) r $@`), no ranlib.
3. `CCVER` differs per machine (`3.6.0` default, `/opt/sdcc-4.6.0` via `CCDIR`);
   `CCDIR=/usr` means the system `sdcc` on `PATH`.
4. Library flags go at the end (`$(LIBS)` after objects), as for gcc.
5. `-p` takes the lowercase chip *with* the family prefix (`16f876a`).

## 5. MCC18 (legacy PIC18)

* discovery on Windows through the registry:
  `reg QUERY 'HKCU\Software\Microchip\MPLAB IDE\Tool Locations' /v T_MCC18.PIC18`,
  Linux `/opt/microchip/mcc18`;
* compile `mcc18 $(CFLAGS) $(CPPFLAGS) -c $<` with `-D__<chip>=1`,
  `-I$(CCDIR)/include`; link with `mplink` (needs a `.lkr`); `mplib` for libraries;
* outputs `.o .hex .cof .map .sym .lst .hxl .rlf .sdb .pre` — the `clean` rule
  of `mcc18.mk` is the best list of what the toolchain leaves around.

Support level: **keep as a descriptor** (maintained by data only); no new work.

## 6. gputils / MPASM

* `gpasm -p<chip> file.asm -o out.hex` as MPLAB X calls it; `gplink -m -s <lkr>`;
* chip name for gputils is `p16f1454` / `-p16f1454`;
* includes are searched in `/usr/share/gputils/header`;
* assembly dependency scan reads `include "x.inc"` and `#include`.

## 7. MPLAB X

### 7.1 `configurations.xml` → toolchain options

The mapping (property key → XC8 v1 flag → XC8 v2 flag) is the table to put in
the descriptor `[mplabx.map]`:

| property (HI-TECH-COMP) | XC8 v1 | XC8 v2 |
| --- | --- | --- |
| `operation-mode=pro` | `--mode=pro` | – |
| `optimization-level=-Os` | – | `-Os` |
| `favor-optimization-for=-speed,+space` | `--opt=…,-speed,+space` | – |
| `optimization-assembler=true` | `+asm` | – |
| `optimization-assembler-files=true` | `+asmfile` | `-fasmfile` |
| `optimization-debug=false` | `-debug` | – |
| `optimization-speed` | `+speed` | – |
| `optimization-invariant-*`, `optimization-stable-*` | `--opt=…` extras | – |
| `identifier-length=255` | `-N255` | – |
| `warning-level=-3` | `--warn=3` | `-mwarn=-3` |
| `asmlist=true` | `--asmlist` | `-Wa,-a` |
| `preprocess-assembler=true` | `-P` | `-xassembler-with-cpp` |
| `define-macros=A;B` | `-DA -DB` | same |
| `undefine-macros` | `-U…` | same |
| `extra-include-directories` | `-I…` | same |
| `pack-struct`, `short-enums`, `default-char-type` (signed), `default-bitfield-type` | language options (v2 `-fpack-struct`…) | – |
| `garbage-collect-*`, `local-generation` | `--opt=…-local` | – |
| `use-cci`, `opt-xc8-compiler-strict_ansi` | `--std=…` | `-std=c99` etc. |

| property (HI-TECH-LINK) | XC8 v1 | XC8 v2 |
| --- | --- | --- |
| `additional-options-code-offset` | `--codeoffset=` | `-mcodeoffset=` |
| `additional-options-command-line` | verbatim | verbatim |
| `additional-options-checksum` | `--output=…`, `--checksum=` | `-mchecksum=` |
| `additional-options-errata` | `--errata=…` | `-merrata=…` |
| `additional-options-extend-address` | `--EXTEND` | `-mextend-address` |
| `additional-options-trace-type` | `--TRACE…` | – |
| `additional-options-use-response-files` | long command lines via `@file` | – |
| `backup-reset-condition-flags` | `+resetbits` | `-msave-resetbits` |
| `calibrate-oscillator[-value]` | `+osccal` | `-mosccal` |
| `clear-bss` | `+clear` | `-mclear-bss`? (default) |
| `code-model-external=wordwrite` | `--emi=wordwrite` | `-memi=wordwrite` |
| `code-model-rom=00-7FF,800-FFF` | `--rom=…` | `-mrom=…` |
| `data-model-ram` | `--ram=…` | `-mram=…` |
| `data-model-size-of-double=24\|32`, `…-float` | `--double= --float=` | `-fno-short-double -fno-short-float` / `-fshort-…` |
| `display-*` (`overall`, `psect`, `class`, `hex`) | `--summary=…` | `-msummary=…` |
| `fill-flash-options-*` | `--fill=…` | `-mfill=…` |
| `format-hex-file-for-download` | `+download` | `-mdownload` |
| `initialize-data` | `+init` | `-Wl,--data-init` |
| `input-libraries=libm` | `-lm` | `-lm` |
| `keep-generated-startup.as` | `+keep` | `-mkeep-startup` |
| `link-in-c-library`, `link-in-peripheral-library` | `+clib`, `+plib` | |
| `managed-stack` | `--stack=hybrid` | `-mstack=hybrid` |
| `program-the-device-with-default-config-words` | `+config` | `-mdefault-config-bits` |
| `remove-unused-sections` | garbage collection | `-Wl,--gc-sections` |

**Evidence level:** the v1 and v2 columns for `optimization-*`, `warning-level`, `asmlist`, `preprocess-assembler`, `data-model-size-of-*`, `code-model-external`, `additional-options-code-offset`, `display-*`, `initialize-data`, `keep-generated-startup.as`, `format-hex-file-for-download`, `program-the-device-with-default-config-words`, `backup-reset-condition-flags`, `calibrate-oscillator`, `managed-stack`/`--stack` were read off generated `Makefile-<conf>.mk` lines. Rows for `rom`/`ram`/`fill`/`checksum`/`errata`/`extend-address`/`trace`/`remove-unused-sections`/`pack-struct`/`short-enums`/`use-cci` and the cells marked with `?` are **not yet verified** against a generated makefile (no project in the five repositories sets them to a non-default value); the importer must treat them as unknown and carry them verbatim until a fixture proves the mapping.

Where a cell says `–` the property has no equivalent; the adapter records it
and warns (spec 9.10). **Verification:** generate the flags from the properties
and compare with the `Makefile-<conf>.mk` MPLAB X wrote from the same file
(conformance, spec 14.4). All 28 + 6 projects supply test cases.

### 7.2 Layout and files MPLAB X produces

* `build/<conf>/<production|debug>/…` objects, `dist/<conf>/<type>/<proj>.X.<type>.<hex|cof>`,
  `memoryfile.xml`, `.map`; sources outside the project directory appear under
  `_ext/<hash>/…` (e.g. `_ext/671494620/usb.p1` for `../../../../USB/usb.c`) —
  the hash is of the directory; genmakefile mirrors paths instead.
* two image types: **debug (`DEBUG_RUN`)** compiles with `-D__DEBUG=1 -G`/`-g` and
  links to `.cof`; **production** links to `.hex`.
* `Makefile-local-<conf>.mk` holds the machine specific tool paths
  (`MP_CC="/opt/microchip/xc8/v1.45/bin/xc8"`, `MP_CC_DIR`, `DFP_DIR`,
  `MP_JAVA_PATH`, `DEP_GEN=… extractobjectdependencies.jar`): **never** copied
  into a generated build; resolved by compiler discovery (spec 9.13).
* `nbproject/private/*` is user state: not read.

## 8. MPLAB 8 (`.mcp`)

* INI file; `[HEADER] device=PIC16F876A`; `[PATH_INFO]` with `dir_bin`
  (output directory), `dir_inc` (include path, `;` separated, backslashes);
  `[FILE_INFO]`/`[FILE_SUBFOLDERS]` the source list; `[CAT_FILTERS]` the
  patterns (`*.c;*.as;*.asm;*.usb`, objects `*.obj;*.p1;*.hex`, libs `*.lib;*.lpp`);
* **suite GUIDs** identify the toolchain: XC8 `{38171385-97B2-4EC5-BF2C-C2C027BA5B04}`,
  SDCC `{DBB37D42-EAE0-43C3-A936-355514C2170D}` (HI-TECH `{507D93FD-…}` dropped);
  **tool settings** `TS{<tool guid>}=` followed by `hexid=value` pairs:
  `DB` warning level, `D1` defines (comma separated, `%2C` for a comma), `DF`
  verbose, `DD` optimise global, `C2` optimise speed, `C3` optimise debug, `DE`
  optimise assembler, `C6` identifier length, `D7` preprocess assembler, `DC`
  optimisation level, `FE` debugger (39 PICkit3, 31 auto), `EC` clear BSS, `F0`
  keep startup, `EF` init data, `F8` calibrate oscillator, `F9` reset bits, `FA`
  download format, `C1` managed stack, `10E` default config words, `110` link
  peripheral lib, `E3` additional command line (commas for spaces), `E5`
  memory model, `E8` size of double (0=24, 1=32); unknown ids are carried verbatim.
  The writer exists (`src/genmakefile/mplab.c`); a **reader** completes the
  importer for old projects.

## 9. Programmers (for `flash` targets)

`picpgm -if JDM -p PIC<CHIP> -e -p <hex>`, `picprog --jdm --device=pic<chip>
--input-hexfile=<hex> --erase --burn`, `pk3cmd.sh`, `ipecmd.sh`,
`PM3Cmd`; `README.ipecmd` documents the author's ICSP wiring. A descriptor
`[flash]` section holds the command templates; `make program`, `verify`, `report`
exist in `common.mk` and become standard targets (spec 10).

## 10. Cross-cutting quirks the generator must encode

| # | quirk | consequence in genmakefile |
| --- | --- | --- |
| 1 | compile from inside the object dir (XC8 v1) | `compile-cwd`, path rewrite of every path-valued flag and the source |
| 2 | duplicate `-D` is fatal | defines are a name-keyed set; conflict is an error |
| 3 | tool leaves side files (`.cod`, `.lst`, `.sym`, `.rlf`, `.sdb`, `.pre`, `.hxl`, `.as`) | descriptor `[names] byproducts` for clean |
| 4 | more than one output per link (`.hex .cof .elf .map .cfg`) | grouped targets / stamp |
| 5 | hex format differs by family (`inhx32` vs `inhx032`) | per-family flag in descriptor |
| 6 | stack option arity differs by family | per-family string |
| 7 | math library name by port (SDCC) | per-port `libs` |
| 8 | linker script generated from the vendor's | `[rule.lkr]` + discovery of the vendor script |
| 9 | reset stub below the offset (SDCC) | `[rule.hexfilter]` + `check-hex` |
| 10 | per-chip / per-program compiler version | `CCVER` per node |
| 11 | pack (DFP) required for XC8 v2 | discovery of `~/.mchp_packs`, variable `DFP_DIR` |
| 12 | variables must not be expanded by ninja (`$$DFP_DIR`) | backend escape rule |
| 13 | compile and link share flags (XC8) | `[commands] ld` includes `$(CFLAGS_LINK)` |
| 14 | messages format switches (`--errformat`) for editors | optional `diagnostics = gcc` |
| 15 | chip macros differ in case (`PIC18F25K50` vs `__18f25k50`) | `[chip]` generates all spellings |
| 16 | an unsupported chip fails late | `--chipinfo`-based validation when the compiler exists; `config-<chip>.h` list otherwise |
| 17 | quoting values with spaces in `@file` | argfile writer quotes `--opt="…"` forms |
| 18 | Windows paths with backslashes in tool output | normalise `\` to `/` when reading `.dep` |
| 19 | library tool is the compiler driver (XC8 v1) | `ar = $(CC) --OUTPUT=lpp …` |
| 20 | tool names vary by OS (`xc8` vs `xc8.exe`, `xc8-cc`) | candidate lists |
