#include "../xml.h"
#include <stdarg.h>

xmlnode*
xml_pfind_pred(xmlnode* node, int (*pred)(xmlnode*, void*), void* vptr[]) {
  do {
    if(pred(node, vptr)) break;

    if(node->children) {
      xmlnode* r;

      if((r = xml_pfind_pred(node->children, pred, vptr)))
        return r;
    }
  } while((node = node->next));

  return node;
}

xmlnode*
xml_vfind_pred(xmlnode* node, int (*pred)(xmlnode*, void*), va_list args) {
  void* vptr[4];
  vptr[0] = va_arg(args, void*);
  vptr[1] = va_arg(args, void*);
  vptr[2] = va_arg(args, void*);
  vptr[3] = va_arg(args, void*);

  return xml_pfind_pred(node, pred, vptr);
}

xmlnode*
xml_find_pred(xmlnode* node, int (*pred)(xmlnode*, void*), ...) {
  xmlnode* ret;
  va_list args;
  va_start(args, pred);

  ret = xml_vfind_pred(node, pred, args);

  va_end(args);

  return ret;
}
