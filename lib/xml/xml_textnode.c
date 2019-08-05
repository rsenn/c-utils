#include "../xml.h"
#include "../str.h"

xmlnode*
xml_textnode(const char* text, size_t len) {
  xmlnode* node;

  if((node = xml_newnode(XML_TEXT))) {
    node->name = str_ndup(text, len);
  }
  return node;
}

