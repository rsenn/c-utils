#include "../xml.h"
#include "../str.h"

char*
xml_get_attribute(xmlnode* node, const char* attr) {
  if(node->attributes) {
    TUPLE* data;

    if(hmap_search(node->attributes, (const void*)attr, str_len(attr), &data) == HMAP_SUCCESS)
      return data->vals.val_chars;
  }
  return NULL;
}
