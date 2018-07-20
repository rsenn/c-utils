#include "../xml.h"

xmlnode*
xml_textnode(const char* text, size_t len) {
  xmlnode* node;
  if((node = xml_newnode(XML_NODE_TEXT))) {
    node->name = str_ndup(text, len);
  }
  return node;
}
