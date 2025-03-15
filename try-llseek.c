#include <sys/types.h>
#include <unistd.h>

int _llseek(unsigned int fd, unsigned long offset_high, unsigned long offset_low, loff_t* result, unsigned int whence);

int
main() {
  loff_t r;
  return _llseek(0, 0, 0, &r, 0);
}
