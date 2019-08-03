#include "../fmt.h"
#include "../tai.h"

size_t
fmt_tai(char* dest, const struct tai* ta) {
  time_t t = (time_t)ta->x;
  return fmt_iso8601(dest, t);
}


