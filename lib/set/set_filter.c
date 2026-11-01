#include "../set.h"

static int
filter(const set_t* in, set_t* out, int (*pred)(const char*, size_t), int want) {
  const bucket_t* b;
  int added = 0;

  for(b = in->list; b; b = b->list_next)
    if(!!pred(b->value, b->size) == want)
      switch(set_add(out, b->value, b->size)) {
        case SET_ADDED: ++added; break;
        case SET_ERROR: return -1;
      }

  return added;
}

/* Adds the values for which pred(value, size) is true to out, in order.
 * Returns how many were added, or -1 if the memory ran out. */
int
set_filter(const set_t* in, set_t* out, int (*pred)(const char*, size_t)) {
  return filter(in, out, pred, 1);
}

/* The same for the values for which pred is false. */
int
set_filter_out(const set_t* in, set_t* out, int (*pred)(const char*, size_t)) {
  return filter(in, out, pred, 0);
}
