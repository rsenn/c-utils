#include "../array.h"
#include "../textcode.h"

void
fmt_tofrom_array2(size_t (*func)(char*, const char*, size_t, const char*), array* dest, array* src, const char* escapeme) {
  size_t needed;
  char* x;

  if(array_failed(dest) || array_failed(src)) {
    array_fail(dest);
    return;
  }
  needed = func(0, (const char*)array_start(src), array_bytes(src), escapeme);
  if(array_bytes(dest) + needed >= needed && array_allocate(dest, 1, array_bytes(dest) + needed - 1)) {
    x = ((char*)array_start(dest)) + array_bytes(dest) - needed;
    func(x, (const char*)array_start(src), array_bytes(src), escapeme);
  } else
    array_fail(dest);
}
