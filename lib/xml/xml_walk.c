#include "../xml.h"

void
xml_walk(xmlnode* root, int (*func)(xmlnode*,xmlnode*)) {
  xmlnode* node = root;

  do {
    if(func(node, root))
      return;

    if(node->children || node->next)
      node = node->children ? node->children : node->next;
    else {
      while((node = node->parent) && node != root && !node->next)
        ;

      node = (node && node->next) ? node->next : 0;
    }
  } while(node);
}
