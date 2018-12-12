#include "../linked_list.h"
#include "../memory.h"
#include "../hashmap.h"
int
hashmap_default_comparator(const void* l, const void* r) {
  return *((unsigned long*)l) - *((unsigned long*)r);
}
