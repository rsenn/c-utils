#include "../xml.h"


const char*
xml_content(xmlnode* node) {
  for(node = node->children; node; node = node->next) {
    if(node->type == XML_TEXT)
      return node->name;
  }
  return NULL;
}
