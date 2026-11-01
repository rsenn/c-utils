#!/bin/sh
# --import-mplabx: chip, defines, include directories and .c files of a MPLAB X project
# usage: import-mplabx.sh /path/to/genmakefile
G=${1:-genmakefile}
d=$(mktemp -d) || exit 1
trap 'rm -rf "$d"' EXIT
cd "$d" || exit 1
mkdir -p P.X/nbproject
cat >P.X/nbproject/configurations.xml <<'XML'
<?xml version="1.0" encoding="UTF-8"?>
<configurationDescriptor version="65">
  <logicalFolder name="root" displayName="root" projectFiles="true">
    <logicalFolder name="SourceFiles" displayName="Source Files" projectFiles="true">
      <itemPath>main.c</itemPath>
      <itemPath>uart.c</itemPath>
    </logicalFolder>
    <logicalFolder name="HeaderFiles" displayName="Header Files" projectFiles="true">
      <itemPath>uart.h</itemPath>
    </logicalFolder>
  </logicalFolder>
  <confs>
    <conf name="default" type="2">
      <toolsSet>
        <targetDevice>PIC18F25K50</targetDevice>
      </toolsSet>
      <XC8-config>
        <property key="define-macros" value="USE_UART=1;BAUD=9600"/>
        <property key="extra-include-directories" value="inc"/>
      </XC8-config>
    </conf>
    <conf name="other" type="2">
      <toolsSet>
        <targetDevice>PIC16F1455</targetDevice>
      </toolsSet>
    </conf>
  </confs>
</configurationDescriptor>
XML
cd P.X || exit 1
printf 'int main(void){return 0;}\n' >main.c
printf 'void u(void){}\n' >uart.c
"$G" -t xc8 -m gmake --import-mplabx nbproject/configurations.xml -o Makefile >/dev/null 2>&1 || { echo "FAIL: genmakefile"; exit 1; }
grep -q '^CHIP = 18f25k50' Makefile || { echo "FAIL: chip of the first configuration"; exit 1; }
grep -q 'USE_UART=1' Makefile && grep -q 'BAUD=9600' Makefile || { echo "FAIL: defines"; exit 1; }
grep -q 'uart.p1' Makefile && grep -q 'main.p1' Makefile || { echo "FAIL: sources"; exit 1; }
grep -q '16f1455' Makefile && { echo "FAIL: second configuration used"; exit 1; }
cd "$d" || exit 1
mkdir -p Q/src
printf 'int main(void){return 0;}\n' >Q/src/main.c
printf '[HEADER]\ndevice=PIC16F1455\n[FILE_INFO]\nfile_000=src\\main.c\nfile_001=src\\p.h\n[TOOL_SETTINGS]\n' >Q/p.mcp
"$G" -t xc8 -m gmake --import-mplabx Q/p.mcp -o Q/Makefile >/dev/null 2>&1 || { echo "FAIL: genmakefile (.mcp)"; exit 1; }
grep -q '^CHIP = 16f1455' Q/Makefile && grep -q 'main.p1' Q/Makefile || { echo "FAIL: .mcp chip or sources"; exit 1; }
echo "import-mplabx: ok"
