#include "../xml.h"

void
xml_insert(xmlnode** nptr, xmlnode* node) {
  node->next = *nptr;
  *nptr = node;
}
