
#include "../xml.h"
#include <stdlib.h>
static size_t
xml_free_node(xmlnode* node) {
  size_t n = 0;
  xmlnode* next;
  do {
    if(node->children)
      n += xml_free_node(node->children);
    if(node->name)
      free(node->name);
    if(node->attributes)
      hmap_destroy(&node->attributes);
    ++n;
    next = node->next;
    free(node);
  } while((node = next));
  return n;
}

void
xml_free(xmlnode* node) {
  size_t num = xml_free_node(node);
#ifdef DEBUG
  buffer_puts(buffer_2, "nodes free'd: ");
  buffer_putulong(buffer_2, num);
  buffer_putnlflush(buffer_2);
#endif
}

