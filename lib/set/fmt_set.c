#include "../set.h"
#include "../str.h"
#include "../byte.h"

size_t
fmt_set(char* out, const set_t* set, const char* separator) {
  char* x;
  size_t i = 0, pos = 0, n, slen = str_len(separator);
  set_iterator_t it;

  set_foreach(set, it, x, n) {
    if(i++ > 0) {
      if(out)
        byte_copy(&out[pos], slen, separator);
      pos += slen;
    }
    if(out)
      byte_copy(&out[pos], n, x);
    pos += n;
  }
  return pos;
}
