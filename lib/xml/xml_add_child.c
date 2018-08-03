#include "../xml.h"

void
xml_add_child(xmlnode* parent, xmlnode* node) {
  xmlnode** nptr = &parent->children;

  node->parent = parent;
  node->next = NULL;

  while(*nptr)
    nptr = &(*nptr)->next;

  *nptr = node;
}
