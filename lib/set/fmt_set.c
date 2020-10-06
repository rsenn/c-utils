#include "../set.h"
#include "../str.h"
#include "../byte.h"

size_t
fmt_set(char* out, const set_t* set, const char* separator) {
  char* x;
  size_t i = 0, n, slen = str_len(separator);
  if(out)
    str_copy(&out[i], "{ ");
  i += 2;
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
  if(out)
    str_copy(&out[i], " }\n");
  i += 3;
  return i;
}
