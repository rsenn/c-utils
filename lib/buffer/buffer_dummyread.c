#include "../buffer.h"

ssize_t
buffer_dummyread(fd_type, void*, size_t, void*) {
  return 0;
}
