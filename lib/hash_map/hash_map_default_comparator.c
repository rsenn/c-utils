#include "../linked_list.h"
#include "../memory.h"
#include "../hash_map.h"
int
hash_map_default_comparator(const void* l, const void* r) {
  return *((unsigned long*)l) - *((unsigned long*)r);
}
