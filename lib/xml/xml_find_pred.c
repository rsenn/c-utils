#include "../xml.h"
#include <stdarg.h>

xmlnode*
xml_find_pred(xmlnode* node, int (*pred)(xmlnode*, void*), ...) {
  va_list args;
  va_start(args, pred);
  void* ptr = va_arg(args, void*);
  do {
    if(pred(node, ptr)) return node;
    if(node->children) {
      xmlnode* r = xml_find_pred(node->children, pred, ptr);
      if(r) return r;
    }
  } while((node = node->next));
  return NULL;
}
