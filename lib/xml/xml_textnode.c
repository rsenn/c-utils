#include "../xml.h"
#include "../str.h"

xmlnode*
xml_textnode(const char* s) {
  xmlnode* node;

  if((node = xml_newnode(XML_TEXT)))
    node->name = str_dup(s);

  return node;
}