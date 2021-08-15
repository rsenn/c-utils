#define _LARGEFILE_SOURCE 1
#define _GNU_SOURCE 1
#define _FILE_OFFSET_BITS 64

#include <sys/stat.h>

//#include <sys/time.h>
#include <time.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "lib/stralloc.h"
#include "lib/buffer.h"
#include "lib/open.h"
#include "lib/fmt.h"
#include "lib/byte.h"
#include "lib/str.h"
#include "lib/io.h"
#include "lib/iarray.h"
#include "lib/path.h"

#ifndef _MAX_PATH
#define _MAX_PATH PATH_MAX
#endif

#ifndef MAX_PATH
#define MAX_PATH _MAX_PATH
#endif

static int skip_fields = 8;
static char* delimiters = " \t\r";
static size_t delimiters_len;

static stralloc dirp = {0, 0, 0};

int
is_delimiter(char c) {
  return !(byte_chr(delimiters, delimiters_len, c) == delimiters_len);
}

size_t
skip_field(int n, char* s, size_t len) {
  size_t ret = 0;
  while(n-- && ret < len) {
    if(ret == len)
      return ret;

    while(ret < len && !is_delimiter(s[ret]))
      ret++;

    if(ret == len)
      return ret;

    while(ret < len && is_delimiter(s[ret]))
      ret++;

    if(ret == len)
      return ret;
  }
  return ret;
}

int
decode_ls_lR() {
  char buffer[MAX_PATH];
  ssize_t pos, len, i;
  size_t offset = dirp.len;
  int is_dir;

  for(;;) {
    is_dir = 0;
    buffer[0] = '\0';
    len = buffer_getline(buffer_0, buffer, sizeof(buffer));

    if(len < 0) /* || buffer[0] == '\0') */
      break;

    if(len == 0 && buffer[0] != '\n') {
      buffer_close(buffer_0);
      // buffer_puts(buffer_2, "len ==
      // 0!!!\n");
      // buffer_flush(buffer_2);
      break;
    }

    if(buffer[len - 1] == ':')
      buffer[len - 1] = '/';

    if(buffer[len - 1] == '/')
      is_dir = 1;

    if(is_dir) {
      dirp.len = offset;
      stralloc_catb(&dirp, buffer, len);
      buffer_put(buffer_1, dirp.s, dirp.len);
      buffer_put(buffer_1, "\n", 1);
      continue;
    }

    pos = skip_field(skip_fields, buffer, len);

    if(pos == len)
      continue;

    buffer_put(buffer_1, dirp.s, dirp.len);

    for(i = len - 4; i > 0 && i >= pos; i--) {
      if(!str_diffn(&buffer[i], " -> ", 4)) {
        len = i;
        break;
      }
    }

    buffer_put(buffer_1, &buffer[pos], len - pos);
    buffer_put(buffer_1, "\n", 1);
  }
  buffer_flush(buffer_1);
  return 0;
}

void
usage(char* arg0) {
  buffer_puts(buffer_2, "Usage: ");
  buffer_puts(buffer_2, str_basename(arg0));
  buffer_puts(buffer_2, " [Options]\n");
  buffer_puts(buffer_2,
              " -s num   Skip <num> "
              "Number of fields\n");
  buffer_flush(buffer_2);
  exit(1);
}

int
main(int argc, char* argv[]) {
  int argi;

  for(argi = 1; argi < argc; argi++) {
    char* arg = argv[argi];
    if(arg[0] == '-') {
      switch(arg[1]) {
        case 's':
          argi++;
          if(argi < argc)
            skip_fields = atoi(argv[argi]);
          break;
        case 'd':
          argi++;
          if(argi < argc) {

            delimiters = argv[argi];
            delimiters_len = str_len(delimiters);
          }
          break;
        case 'p':
          argi++;
          if(argi < argc) {
            stralloc_copys(&dirp, argv[argi]);
            if(dirp.len && dirp.s[dirp.len - 1] != '/')
              stralloc_catb(&dirp, "/", 1);
          }
          break;
        default: usage(argv[0]); break;
      }
    } else
      break;
  }
  if(argi < argc) {
    buffer_puts(buffer_2, "Opening file ");
    buffer_puts(buffer_2, argv[argi]);
    buffer_puts(buffer_2, "...\n");
    buffer_flush(buffer_2);
    if((buffer_0->fd = open(argv[argi], O_RDONLY)) < 0)
      usage(argv[0]);
  }
  delimiters_len = str_len(delimiters);
  decode_ls_lR();
  return 0;
}