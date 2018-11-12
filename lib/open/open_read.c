#define _FILE_OFFSET_BITS 64
#include "../windoze.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

#include "../open.h"
#include <fcntl.h>

#ifndef O_BINARY
#define O_BINARY 0
#endif

int
open_read(const char* filename) {
  return open(filename, O_RDONLY | O_BINARY);
}
