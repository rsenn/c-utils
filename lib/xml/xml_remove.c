#include "../xml.h"

xmlnode*
xml_remove(xmlnode** nptr) {
  xmlnode* node = *nptr;
  *nptr = node->next;
  node->parent = NULL;
  return node;
}
