#include "../buffer.h"
#include "../typedefs.h"

ssize_t
buffer_dummyreadmmap(fd_type, void*, size_t, void*) {
  return 0;
}
