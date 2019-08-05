#include "../xml.h"
#include "../str.h"

xmlnode*
xml_element(const char* name) {
  xmlnode* node = xml_newnode(XML_ELEMENT);
  node->name = str_dup(name);
  return node;
}

