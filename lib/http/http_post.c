#include "../http.h"

int
http_post(http* h, const char* location) {
  return http_open(h, POST, location);
}
