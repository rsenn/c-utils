#include "../uri.h"
#include "../byte.h"

int
uri_init(uri_t* u) {
  byte_zero(u, sizeof(uri_t));
  return 1;
}
