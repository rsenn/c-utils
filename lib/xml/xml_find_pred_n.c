#include "../xml_internal.h"
#include "../strlist.h"
#include <stdarg.h>

static xmlnode*
xml_find_predicate(xmlnode* node, xml_pred_t* pred, const void* vptr[]) {
  while(node) {
    if(pred(node, vptr[0], vptr[1], vptr[2]))
      break;

    if(node->children) {
      xmlnode* r;

      if((r = xml_find_predicate(node->children, pred, vptr)))
        return r;
    }

    node = node->next;
  }

  return node;
}

xmlnode*
xml_find_pred_n(xmlnode* node, xml_pred_t* pred, int n, ...) {
  xmlnode* ret;
  va_list args;
  va_start(args, n);

  ret = xml_vfind_pred_n(node, pred, n, args);

  va_end(args);

  return ret;
}
