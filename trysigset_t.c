#define _GNU_SOURCE
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>

int
main() {
  sigset_t ss;
  printf("%lu\n", sizeof(ss));
  return 0;
}
