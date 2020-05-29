#include "ini.h"

int
main(int argc, char* argv[]) {
  const char* x;
  size_t n;
  buffer input;
  ini_section_t* ini;

  buffer_mmapread(&input, "/home/roman/Sources/pictest/build/mplab/7segtest-16f876a-xc8-debug.mcp");

  ini_read(&input, &ini);
}
