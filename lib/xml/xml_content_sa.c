#include "../xml.h"


int
xml_content_sa(xmlnode* node, stralloc* sa) {
  ssize_t n = sa->len;

  for(node = node->children; node; node = node->next) {
    if(node->type == XML_TEXT) {
      stralloc_cats(sa, node->name);
    }
  }
  return sa->len - n;
}
