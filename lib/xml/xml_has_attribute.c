#include "../xml.h"

int
xml_has_attribute(xmlnode* node, const char* attr) {
  if(node->attributes) {
    TUPLE* data;
    if(hmap_search(node->attributes, (void*)attr, str_len(attr), &data) == HMAP_SUCCESS)
      return 1;
  }
  return 0;
}
