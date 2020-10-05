#include "../fmt.h"
#include "../str.h"

size_t
fmt_str(char* out, const char* in) {
  return str_copy(out, in);
}
