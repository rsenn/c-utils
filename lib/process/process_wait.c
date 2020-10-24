#include "../process.h"

int
process_wait(int pid) {
  int status = -1;
  wait_pid(pid, &status);
  return status;
}