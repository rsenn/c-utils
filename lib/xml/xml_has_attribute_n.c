#include "../xml.h"
#include "../str.h"

int
xml_has_attribute_n(xmlnode* node, const char* b, size_t n) {
  if(node->attributes) {
    TUPLE* data;

    if(hmap_search(node->attributes, (const void*)b, n, &data) == HMAP_SUCCESS)
      return 1;
  }
  return 0;
}
