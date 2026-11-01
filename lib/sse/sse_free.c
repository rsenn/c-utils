#include "../sse.h"

void
sse_free(sse* s) {
  stralloc_free(&s->line);
  stralloc_free(&s->data);
  stralloc_free(&s->event);
  stralloc_free(&s->id);
  s->has_data = 0;
}
