#include "../xml.h"
#include <stdlib.h>

/* frees node and everything below it; does not touch the siblings or the parent */
static void
xml_delete_tree(xmlnode* node) {
  xmlnode *child, *next;

  /* the next pointer is taken before the child is freed (the loop used to read it
   * from the freed child, and each child unlinked itself from the list it was
   * being walked in) */
  for(child = node->children; child; child = next) {
    next = child->next;
    xml_delete_tree(child);
  }
  if(node->name)
    free(node->name);
  if(node->attributes)
    hmap_destroy(&node->attributes);
  free(node);
}

/* removes node from the children of its parent and frees it with all it contains */
void
xml_delete(xmlnode* node) {
  if(node->parent) {
    xmlnode** nptr = &node->parent->children;

    while(*nptr) {
      if(*nptr == node) {
        *nptr = node->next;
        break;
      }
      nptr = &(*nptr)->next;
    }
  }
  xml_delete_tree(node);
}
