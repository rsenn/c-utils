#include "../textcode.h"
#include "../array.h"

/* decodes the contents of src (taken as a string) and appends the result to dest */
size_t
scan_tofrom_array(size_t (*func)(const char*, char*, size_t*), array* src, array* dest) {
  size_t scanned = 0, ret, needed;
  int64 old;
  char* x;

  array_cat0(src);
  if(array_failed(src) || array_failed(dest))
    return 0;
  needed = array_bytes(src);
  old = array_bytes(dest);
  if(!array_allocate(dest, 1, old + needed - 1)) {
    array_truncate(src, 1, array_bytes(src) - 1);
    return 0;
  }
  x = (char*)array_start(dest) + old;
  ret = func((const char*)array_start(src), x, &scanned);
  array_truncate(dest, 1, ret ? old + scanned : old);
  array_truncate(src, 1, array_bytes(src) - 1);
  return ret;
}
