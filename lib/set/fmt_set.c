#include "../set.h"
#include "../str.h"
#include "../byte.h"

size_t
fmt_set(char* out, const set_t* set, const char* separator) {
  char* x;
  size_t i = 0, n, slen = str_len(separator);
  set_foreach(set, x, n) {
    if(i++ > 0) {
      if(out)
        byte_copy(&out[i], slen, separator);
      i += slen;
    }
    if(out)
      byte_copy(&out[i], n, x);
    i += n;
  }
  return i;
}
