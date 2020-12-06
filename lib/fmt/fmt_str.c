#include "../fmt.h"
#include "../str.h"

size_t
fmt_str(char* out, const char* in) {
  if(out)
    return str_copy(out, in);
  return str_len(in);
}
