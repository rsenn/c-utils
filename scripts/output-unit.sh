output-unit () 
{ 
    while [ $# -gt 0 ]; do
        echo "                <Unit filename=\"$1\"><Option compilerVar=\"CC\"/></Unit>";
        shift;
    done
}

output-cbp() {
 (NAME="$1"
  shift
  unset UNITS
  for UNIT; do
    pushv UNITS "    <Unit filename=\"$UNIT\"><Option compilerVar=\"CC\"/></Unit>"
  done
cat <<EOF
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<CodeBlocks_project_file>
  <FileVersion major="1" minor="6"/>
  <Project>
    <Option title="$NAME"/>
    <Option pch_mode="2"/>
    <Option compiler="gcc"/>
    <Build>
      <Target title="Debug">
        <Option output="bin/Debug/$NAME" prefix_auto="1" extension_auto="1"/>
        <Option object_output="obj/Debug/"/>
        <Option type="1"/>
        <Option compiler="gcc"/>
        <Compiler>
          <Add option="-Wextra"/>
          <Add option="-Wall"/>
          <Add option="-g3"/>
          <Add option="-ggdb"/>
          <Add option="-O0"/>
          <Add option="-DUSE_WIDECHAR=0"/>
          <Add option="-DDEBUG"/>
          <Add directory="."/>
        </Compiler>
      </Target>
      <Target title="Release">
        <Option output="bin/Release/$NAME" prefix_auto="1" extension_auto="1"/>
        <Option object_output="obj/Release/"/>
        <Option type="1"/>
        <Option compiler="gcc"/>
        <Compiler>
          <Add option="-w"/>
          <Add option="-fomit-frame-pointer"/>
          <Add option="-fexpensive-optimizations"/>
          <Add option="-Os"/>
          <Add option="-ffast-math"/>
          <Add option="-DUSE_WIDECHAR=0"/>
          <Add option="-DNDEBUG=1"/>
          <Add directory="."/>
        </Compiler>
      </Target>
    </Build>
    <Compiler>
      <Add option="-DINLINE=inline"/>
      <Add option="-D_FILE_OFFSET_BITS=64"/>
      <Add option="-D_LARGEFILE_SOURCE=1"/>
      <Add option="-D_GNU_SOURCE=1"/>
      <Add option="-DHAVE_LIBBZ2=1"/>
      <Add option="-DHAVE_LIBLZMA=1"/>
      <Add option="-DHAVE_LIBZ=1"/>
    </Compiler>
    <Linker>
      <Add library="z"/>
      <Add library="bz2"/>
      <Add library="lzma"/>
      <Add library="advapi32"/>
      <Add library="ws2_32"/>
      <Add library="iphlpapi"/>
      <Add library="psapi"/>
      <Add library="shlwapi"/>
    </Linker>
$UNITS
    <Extensions>
      <envvars/>
      <code_completion/>
      <lib_finder disable_auto="1"/>
      <debugger/>
    </Extensions>
  </Project>
</CodeBlocks_project_file>
EOF
)
}
