#include "../set.h"
#include "../str.h"
#include "../byte.h"

size_t
fmt_set(char* out, const set_t* set, const char* separator) {
  size_t i = 0, pos = 0, slen = str_len(separator);
  bucket_t* b;

  for(b = set->list; b; b = b->list_next) {
    if(i++ > 0) {
      if(out)
        byte_copy(&out[pos], slen, separator);
      pos += slen;
    }
    if(out)
      byte_copy(&out[pos], b->size, b->value);
    pos += b->size;
  }
  return pos;
}
