#include "../xml.h"
#include <stdlib.h>

void
xml_delete(xmlnode* node) {
  if(node->children) {
    xmlnode* child = node->children;
    do {
      xml_delete(child);
    } while((child = child->next));
  }

  if(node->name)
    free(node->name);
  if(node->attributes)
    hmap_destroy(&node->attributes);

  if(node->parent) {
    xmlnode** nptr = &node->parent->children;

    do {
      if(*nptr == node) {
        *nptr = node->next;
        break;
      }
      nptr = &(*nptr)->next;
    } while(*nptr);
  }

  free(node);
}
