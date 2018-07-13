e <stdlib.h>
#include <string.h>
#include "map.h"

struct map_node_t {
  unsigned hash;
  void *value;
  map_node_t *next;
  /* char key[]; */
  /* char value[]; */
