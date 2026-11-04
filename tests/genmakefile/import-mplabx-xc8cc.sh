#!/bin/sh
# --import-mplabx with -t xc8-cc (XC8 v2+): the flags of the MPLAB X properties, --code-offset,
# -Os only, no doubled -D__chip, the .hex as a target, clean, and -t xc8 (v1) untouched.
# fixture after USB-Stack CDC_Serial_UART_Example.X (PIC18F25K50, XC8 2.31, offset 0x2000)
# usage: import-mplabx-xc8cc.sh /path/to/genmakefile
G=${1:-genmakefile}
d=$(mktemp -d) || exit 1
trap 'rm -rf "$d"' EXIT
cd "$d" || exit 1
mkdir -p P.X/nbproject
cat >P.X/nbproject/project.xml <<'XML'
<project><configuration><data><name>Prog</name></data></configuration></project>
XML
cat >P.X/nbproject/configurations.xml <<'XML'
<configurationDescriptor version="65">
  <logicalFolder name="root">
    <itemPath>main.c</itemPath>
  </logicalFolder>
  <confs>
    <conf name="C" type="2">
      <toolsSet>
        <targetDevice>PIC18F25K50</targetDevice>
        <languageToolchainVersion>2.31</languageToolchainVersion>
      </toolsSet>
      <packs>
        <pack name="PIC18F-K_DFP" vendor="Microchip" version="1.3.84"/>
      </packs>
      <HI-TECH-COMP>
        <property key="optimization-level" value="-Os"/>
        <property key="warning-level" value="-3"/>
        <property key="asmlist" value="true"/>
      </HI-TECH-COMP>
      <HI-TECH-LINK>
        <property key="additional-options-code-offset" value="0x2000"/>
        <property key="code-model-external" value="wordwrite"/>
        <property key="create-html-files" value="false"/>
        <property key="data-model-size-of-double" value="32"/>
        <property key="data-model-size-of-float" value="32"/>
        <property key="display-class-usage" value="false"/>
        <property key="display-hex-usage" value="false"/>
        <property key="display-overall-usage" value="true"/>
        <property key="display-psect-usage" value="false"/>
        <property key="format-hex-file-for-download" value="false"/>
        <property key="initialize-data" value="true"/>
        <property key="keep-generated-startup.as" value="false"/>
        <property key="program-the-device-with-default-config-words" value="true"/>
      </HI-TECH-LINK>
      <XC8-config-global>
        <property key="gcc-opt-std" value="-std=c90"/>
        <property key="stack-size-high" value="auto"/>
        <property key="stack-size-low" value="auto"/>
        <property key="stack-size-main" value="auto"/>
        <property key="stack-type" value="compiled"/>
      </XC8-config-global>
    </conf>
  </confs>
</configurationDescriptor>
XML
cd P.X || exit 1
printf '#define _XTAL_FREQ 1\nint main(void){return 0;}\n' >main.c
"$G" -t xc8-cc -m gmake --import-mplabx nbproject/configurations.xml -d build -o Makefile >/dev/null 2>err || { echo "FAIL: genmakefile"; exit 1; }
f=$(grep -e '^CFLAGS' -e '^LDFLAGS' Makefile)
for w in -mcodeoffset=0x2000 -fno-short-double -fno-short-float -memi=wordwrite -mwarn=-3 -mno-keep-startup \
  -mno-download -mdefault-config-bits -maddrqual=ignore -ginhx032 -std=c99 -mstack=compiled:auto:auto:auto \
  -msummary=-psect,-class,+mem,-hex,-file -Wl,--data-init -Wl,--defsym=__MPLAB_BUILD=1 \
  -Wl,--memorysummary,\$\(BUILDDIR\)memoryfile.xml -Wl,-Map=\$\(BUILDDIR\)Prog.map -Wa,-a; do
  echo "$f" | grep -q -e "$w" || { echo "FAIL: flag $w"; exit 1; }
done
echo "$f" | grep -q -e '-std=c90' && { echo "FAIL: -std=c90 kept"; exit 1; }
grep -q 'warning 1316' err || { echo "FAIL: no c90 warning"; exit 1; }
echo "$f" | grep -q -e '-O1' && { echo "FAIL: -Os -O1"; exit 1; }
[ "$(grep -c -e '-D__18f25k50=1' Makefile)" = 1 ] || { echo "FAIL: -D__18f25k50 twice"; exit 1; }
grep -q '^: ' Makefile && { echo "FAIL: stray rule line"; exit 1; }
grep -q '^DFP_DIR\|DFP_DIR is not set' Makefile || { echo "FAIL: DFP_DIR"; exit 1; }
grep -q 'build/Prog.hex' Makefile || { echo "FAIL: .hex target"; exit 1; }
grep -q 'build/\*.map' Makefile && grep -q 'build/\*.sdb' Makefile || { echo "FAIL: clean"; exit 1; }
grep -q "Prog-universal.hex" Makefile && grep -q "vectors -a .(CODE_OFFSET)" Makefile || { echo "FAIL: universal hex rule"; exit 1; }
"$G" -t xc8-cc -m gmake --import-mplabx nbproject/configurations.xml --no-universal-hex -d build -o Makefile.nu >/dev/null 2>&1
grep -q "Prog-universal" Makefile.nu && { echo "FAIL: --no-universal-hex"; exit 1; }
# the other backends: universal rule and defaults
gen() { m=$1; o=$2; shift 2; "$G" -t xc8-cc -m "$m" --import-mplabx nbproject/configurations.xml -d build -o "$o" "$@" >/dev/null 2>&1; }
gen ninja build.ninja || { echo "FAIL: -m ninja"; exit 1; }
grep -q '^rule ihextool_vectors' build.ninja && grep -q 'Prog-universal.hex: ihextool_vectors' build.ninja &&
  grep -q 'Prog.elf | .*Prog.hex: link' build.ninja && grep -q 'IHEXTOOL:-ihextool' build.ninja &&
  grep -q 'CODE_OFFSET:-0x2000' build.ninja || { echo "FAIL: ninja universal"; exit 1; }
grep -q 'DFP_DIR' build.ninja || { echo "FAIL: ninja DFP_DIR"; exit 1; }
grep -q 'memoryfile.xml' build.ninja && ! grep -q '\$builddir' build.ninja || { echo "FAIL: ninja BUILDDIR"; exit 1; }
gen shell build.sh || { echo "FAIL: -m shell"; exit 1; }
grep -q '^: ${IHEXTOOL:=ihextool}' build.sh && grep -q '^: ${CODE_OFFSET:=0x2000}' build.sh &&
  grep -q '^\$IHEXTOOL vectors -a \$CODE_OFFSET' build.sh || { echo "FAIL: shell universal"; exit 1; }
gen batch build.bat || { echo "FAIL: -m batch"; exit 1; }
grep -q 'IF NOT DEFINED IHEXTOOL SET IHEXTOOL=ihextool' build.bat && grep -q 'IF NOT DEFINED CODE_OFFSET SET CODE_OFFSET=0x2000' build.bat &&
  grep -q '^%IHEXTOOL% vectors -a %CODE_OFFSET%' build.bat || { echo "FAIL: batch universal"; exit 1; }
for m in ninja shell batch; do
  gen $m nu.$m --no-universal-hex
  grep -q 'Prog-universal\|ihextool_vectors\|IHEXTOOL vectors' nu.$m && { echo "FAIL: --no-universal-hex ($m)"; exit 1; }
done
"$G" -t xc8 -m gmake --import-mplabx nbproject/configurations.xml -o Makefile.v1 >/dev/null 2>&1 || { echo "FAIL: -t xc8"; exit 1; }
grep -q -e '-mcodeoffset\|-mdfp\|--cflag' Makefile.v1 && { echo "FAIL: v2 flags in -t xc8"; exit 1; }
echo "import-mplabx-xc8cc: ok"
