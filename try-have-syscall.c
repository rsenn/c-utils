#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>

static inline ssize_t
sys_write(int fd, const void* buf, size_t n) {
  return syscall(SYS_write, fd, buf, n);
}

int
main() {
  return sys_write(1, "test\n", 5);
}
