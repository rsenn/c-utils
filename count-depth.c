#define _LARGEFILE_SOURCE 1
#define _GNU_SOURCE 1
#define _FILE_OFFSET_BITS 64
#if !(defined(_WIN32) || defined(_WIN64))
#include <unistd.h>
#else
#include <io.h>
#endif
#include <limits.h>
#include "lib/byte.h"
#include "lib/buffer.h"
#include "lib/fmt.h"
#include "lib/str.h"
#include "lib/io.h"
#include "lib/iarray.h"

#ifndef MAXIMUM_PATH_LENGTH
#define MAXIMUM_PATH_LENGTH (4096 - 1)
#endif

/*
static char buffer_0_in[BUFFER_INSIZE];
static buffer buffer_0 =
BUFFER_INIT((void*)read, 0, buffer_0_in,
BUFFER_INSIZE);

static char
buffer_1_out[BUFFER_OUTSIZE]; static
buffer buffer_1 =
BUFFER_INIT((void*)write, 1,
buffer_1_out, BUFFER_OUTSIZE);
*/

int
count_depth() {
  char buffer[MAXIMUM_PATH_LENGTH];
  size_t len, i, c;

  for(;;) {
    buffer[0] = '\0';
    len = buffer_getline(buffer_0, buffer, sizeof(buffer));

    if(len == 0 || buffer[0] == '\0')
      break;

    if(buffer[len - 1] == '/')
      len--;

    c = 0;

    for(i = 0; i < len; i++) {
      if(buffer[i] == '/')
        c++;
    }
    buffer_putulong(buffer_1, (unsigned long)c);
    buffer_put(buffer_1, " ", 1);
    buffer_put(buffer_1, buffer, len);
    buffer_put(buffer_1, "\n", 1);
  }
  buffer_flush(buffer_1);
  return 0;
}

int
main(int argc, char* argv[]) {
  int argi = 1;

  while(argi < argc) {
    /*    if(!str_diff(argv[argi], "-l")
       || !str_diff(argv[argi],
       "--list")) { opt_list = 1; }
       else*/
    { break; }
    argi++;
  }
  return count_depth();
}
