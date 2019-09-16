#include "../xml.h"

xmlnode*
xml_get_document(xmlnode* node) {
  while(node->parent) node = node->parent;
  return node;
}
