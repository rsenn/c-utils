#include "../xml.h"

void
xml_walk(xmlnode* root, int (*func)()) {
  xmlnode* node = root;

  do {
    if(func(node,root))
      return;

    if(node->children || node->next)
      node = node->children || node->next;
    else {
      while((node = node->parent) && node != root && !node->next);
      
      node = node && node->next;
    }
  } while(node);
}
