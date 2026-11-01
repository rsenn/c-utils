#include "../http.h"

int
http_get(http* h, const char* location) {
  return http_open(h, GET, location);
}
