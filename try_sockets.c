#include <winsock2.h>
int main() {
  FD_SET w,r,e;
  struct timeval tv;
  select(10, r, w, e, &rv);
  }