#include "../buffer.h"

typedef struct stralloc_s {
  char* s;
  size_t len;
  size_t a;
} stralloc;

int
buffer_putsaflush(buffer* b, const stralloc* sa) {
  return buffer_putflush(b, sa->s, sa->len);
}
