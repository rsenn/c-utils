#include "ini.h"
#include "map.h"
#include "lib/open.h"

int
main(int argc, char* argv[]) {
  const char* x;
  size_t n;
  buffer input, output;
  ini_section_t *ini, *ptr;

  buffer_mmapread(&input, argv[1] ? argv[1] : "/home/roman/Sources/pictest/build/mplab/7segtest-16f876a-xc8-debug.mcp");

  ini_read(&input, &ini);

  for(ptr = ini; ptr; ptr = ptr->next) {

    buffer_puts(buffer_2, "section ");
    buffer_putsa(buffer_2, &ptr->name);
    buffer_putnlflush(buffer_2);
    MAP_DUMP(ptr->map, buffer_2);
    buffer_putnlflush(buffer_2);
  }

  buffer_write_fd(&output, open_trunc("output.ini"));

  ini_write(&output, ini, 0);
}
