#include "../fmt.h"
#include "../tai.h"

size_t
fmt_tai(char* dest, const struct tai* ta) {
  time_t t = (time_t)ta->x;
  uint64 mask = 1;
  mask <<= 62;
  mask = ~mask;
  return fmt_iso8601(dest, t & mask);
}
