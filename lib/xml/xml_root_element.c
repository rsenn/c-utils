#include "../xml.h"
#include <ctype.h>
xmlnode*
xml_root_element(xmlnode* node) {
  for(node = node->children; node; node = node->next) {

    if(node->type == XML_ELEMENT && isalnum(node->name[0])) {
      if(node->children)
        return node;
    }
  }
  return NULL;
}
