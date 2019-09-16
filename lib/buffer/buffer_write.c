#include "../windoze.h"
#include "../buffer.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

int
buffer_unixwrite(int fd, const char* buf, unsigned int len) {
  return write(fd, buf, len);
}
