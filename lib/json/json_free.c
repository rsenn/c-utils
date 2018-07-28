#include "../json.h"
#include <stdlib.h>

static size_t
json_free_node(jsonnode* node) {
}

void
json_free(jsonnode* node) {
  size_t num = json_free_node(node);

#ifdef DEBUG
  buffer_puts(buffer_2, "nodes free'd: ");
  buffer_putulong(buffer_2, num);
  buffer_putnlflush(buffer_2);
#endif
}
