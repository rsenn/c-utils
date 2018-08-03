#include "../xml.h"

xmlnode*
xml_find_pred(xmlnode* node, int (*pred)(xmlnode*, void*), void* ptr) {
  do {
    if(pred(node, ptr)) break;

    if(node->children) {
      xmlnode* r;

      if((r = xml_find_pred(node->children, pred, ptr)))
        return r;
    }
  } while((node = node->next));

  return node;
}
