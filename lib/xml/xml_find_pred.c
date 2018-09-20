#include "../xml.h"
#include <stdarg.h>

xmlnode*
xml_pfind_pred(xmlnode* node, int (*pred)(), void* vptr[]) {
  do {
    if(pred(node, vptr[0], vptr[1], vptr[2], vptr[3])) break;

    if(node->children) {
      xmlnode* r;

      if((r = xml_pfind_pred(node->children, pred, vptr))) return r;
    }
  } while((node = node->next));

  return node;
}

xmlnode*
xml_find_pred_1(xmlnode* node, int (*pred)(/*xmlnode*,void*,*/), void* arg) {
  void* vptr[] = {arg, NULL};
  return xml_pfind_pred(node, pred, vptr);
}

xmlnode*
xml_find_pred_2(xmlnode* node, int (*pred)(/*xmlnode*,void*,void**/), void* a0, void* a1) {
  void* vptr[] = {a0, a1, NULL};
  return xml_pfind_pred(node, pred, vptr);
}

xmlnode*
xml_find_pred_3(xmlnode* node, int (*pred)(/*xmlnode*,void*,void*,void**/), void* a0, void* a1, void* a2) {
  void* vptr[] = {a0, a1, a2, NULL};
  return xml_pfind_pred(node, pred, vptr);
}

xmlnode*
xml_vfind_pred_n(xmlnode* node, int (*pred)(xmlnode*, void*), int n, va_list args) {
  void* vptr[4];
  int i;
  for(i = 0; i < n; ++i) {
    vptr[i] = va_arg(args, void*);
  }
  return xml_pfind_pred(node, pred, vptr);
}

xmlnode*
xml_find_pred_n(xmlnode* node, int (*pred)(xmlnode*, void*), int n, ...) {
  xmlnode* ret;
  va_list args;
  va_start(args, pred);

  ret = xml_vfind_pred_n(node, pred, n, args);

  va_end(args);

  return ret;
}
