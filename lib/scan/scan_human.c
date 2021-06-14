#include "../scan.h"

size_t
scan_human(const char* src, uint64* dest) {
  size_t n = scan_ulonglong(src, dest);

  switch(src[n]) {
    case 'G': *dest *= 1024;
    case 'M': *dest *= 1024;
    case 'K':
    case 'k':
      *dest *= 1024;
      n++;
      break;
    default: break;
  }
  return n;
}
