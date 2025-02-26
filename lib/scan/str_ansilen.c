#undef __dietlibc__
#include "../str.h"
#include "../scan.h"

size_t
str_ansilen(const char* in) {
  size_t i, len = 0;
  size_t slen = str_len(in);

  for(i = 0; i < slen;) {
    size_t p = scan_ansiskip(&in[i], slen - i);

    if(p > 0) {
      i += p;
      continue;
    }
    ++i;
    ++len;
  }
  return len;
}
