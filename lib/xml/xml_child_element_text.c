#include "../xml.h"
#include "../str.h"
#include <stdarg.h>

xmlnode*
xml_child_element_text(const char* name, xmlnode* parent, const char* text) {
  va_list args;
  xmlnode* node = xml_child_element(name, parent);
  node->children = xml_textnode(text);
  return node;
}