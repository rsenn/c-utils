#include "../textcode.h"
#include "../array.h"
#include "../str.h"

/* decodes the string src and appends the result to dest; returns what func
 * returns (the number of characters consumed), 0 on failure */
size_t
scan_to_array(size_t (*func)(const char*, char*, size_t*), const char* src, array* dest) {
  size_t scanned = 0, ret, needed = str_len(src);
  int64 old = array_bytes(dest);
  char* x;

  if(old < 0 || needed == 0)
    return 0;
  /* decoding never makes the text longer */
  if(!(x = array_allocate(dest, 1, old + needed - 1)))
    return 0;
  x = (char*)array_start(dest) + old;
  ret = func(src, x, &scanned);
  array_truncate(dest, 1, ret ? old + scanned : old);
  return ret;
}
