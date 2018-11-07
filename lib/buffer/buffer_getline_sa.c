#include "../buffer.h"

typedef struct stralloc_s {
  char* s;
  size_t len;
  size_t a;
} stralloc;

int
buffer_getline_sa(buffer* b, stralloc* sa) {
  return buffer_get_token_sa(b, sa, "\n", 1);
}
