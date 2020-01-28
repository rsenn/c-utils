#include "../xml.h"
#include "../str.h"

xmlnode*
xml_child_element(const char* name, xmlnode* parent) {
  xmlnode* node = xml_newnode(XML_ELEMENT);
  node->name = str_dup(name);
  xml_add_child(parent, node);
  return node;
}
