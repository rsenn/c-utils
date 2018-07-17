#include "../cbmap_internal.h"

int
cbmap_visit_all(cbmap_t map, cbmap_visitor visitor_fn, void* user_data) {
  uint8_t empty_prefix[1];
  return cbmap_visit_prefix(map, empty_prefix, 0, visitor_fn, user_data);
}
