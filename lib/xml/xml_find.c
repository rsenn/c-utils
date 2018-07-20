#include "../xml.h"

xmlnode*
xml_find(xmlnode* node, const char* tag) {
  do {
    if(node->type != XML_NODE_ELEMENT) continue;

    if(!str_diff(node->name, tag)) return node;

    if(node->children) {
      xmlnode* r = xml_find(node->children, tag);
      if(r) return r;
    }
  } while((node = node->next));

  return NULL;
}
