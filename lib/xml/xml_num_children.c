#include "../xml.h"

size_t
xml_num_children(xmlnode* node) {
  xmlnode* n;
  size_t ret = 0;

  for(n = node->children; n; n = n->next)
    ret++;
  return ret;
}
