#include "../cpp_internal.h"
#include "../str.h"
#include "../open.h"
#include "../seek.h"
#include "../errmsg.h"
#include "../alloc.h"
#include <stdio.h>
#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

/* Returns the contents of a given file. */
char*
cpp_read_file(char* path) {
  fd_type fd;
  seek_pos size;

  if(str_equal(path, "-")) {
    fd = 0;
    size = -1;
  } else {
    if((fd = open_read(path)) < 0) {
      errmsg_infosys("open", 0);
      return 0;
    }
    size = seek_end(fd);
  }

  if(size < 0) {
    stralloc sa;
    stralloc_init(&sa);

    if(readclose(fd, &sa, 4096) == -1) {
      errmsg_infosys("readclose", 0);
      stralloc_free(&sa);
      close(fd);
      return 0;
    }

    stralloc_catc(&sa, '\0');
    close(fd);
    return sa.s;
  }

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "file size: ");
  buffer_putlong(buffer_2, size);
  buffer_putnlflush(buffer_2);
#endif

  seek_begin(fd);
  char* s = alloc(size + 1);

  if(read(fd, s, size) == size) {
    s[size] = '\0';
  } else {
    alloc_free(s);
    s = 0;
  }

  return s;
}
