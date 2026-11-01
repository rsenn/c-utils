#include "../fmt.h"
#include "../str.h"
#include "../byte.h"

size_t
fmt_str(char* out, const char* in) {
  /* libowfat contract: no terminating NUL is written */
  size_t n = str_len(in);

  if(out)
    byte_copy(out, n, in);
  return n;
}
