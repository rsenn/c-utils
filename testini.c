#include "ini.h"
#define MAP_USE_HMAP 1
#include "lib/map.h"
#include "lib/open.h"
#include "lib/env.h"
#include "lib/path.h"

int
main(int argc, char* argv[]) {
  const char* x;
  size_t n;
  buffer input, output;
  ini_section_t *ini, *ptr;
  stralloc file;

  stralloc_init(&file);
  stralloc_copys(&file, path_gethome(getuid()));
  stralloc_cats(&file, "/Sources/pictest/build/mplab/7segtest-16f876a-xc8-debug.mcp");
  stralloc_nul(&file);
  buffer_mmapread(&input, argv[1] ? argv[1] : file.s);
  stralloc_free(&file);

  ini_read(&input, &ini);

  for(ptr = ini; ptr; ptr = ptr->next) {

    /*   buffer_puts(buffer_1, "[");
       buffer_putsa(buffer_1, &ptr->name);
       buffer_puts(buffer_1, "]");

       buffer_putnlflush(buffer_1);*/

    ini_write(buffer_1, ptr, 0);
    MAP_DUMP(ptr->map, buffer_1);
    buffer_flush(buffer_1);
  }

  buffer_write_fd(&output, open_trunc("output.ini"));

  ini_write(&output, ini, 0);
}
