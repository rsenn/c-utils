#include <sys/types.h>
#include <sys/time.h>
#if defined(_WIN32) || defined(__MINGW32__)
#include <winsock2.h>
#else
#include <sys/select.h>
#include <unistd.h>
#endif

int
main() {
  struct timeval tv;
  fd_set rfds;
  int fd = 0;
  FD_ZERO(&rfds);
  FD_SET(fd, &rfds);
  tv.tv_sec = tv.tv_usec = 0;
  select(fd + 1, &rfds, 0, 0, &tv);
}
