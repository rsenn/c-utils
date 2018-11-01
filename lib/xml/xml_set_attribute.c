#include "../xml.h"
#include "../hmap.h"
#include "../str.h"

void
xml_set_attribute(xmlnode* node, const char* a, const char* v) {
  if(node->attributes == NULL)
    hmap_init(1024, &node->attributes);

  hmap_set(&node->attributes, (char*)a, str_len(a), (const void*)v, str_len(v) + 1);
}
