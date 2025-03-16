#include "../buffer.h"
#include "../typedefs.h"

ssize_t
buffer_dummyreadmmap(fd_type fd, void* buf, size_t len, buffer* b) {
  return 0;
}
