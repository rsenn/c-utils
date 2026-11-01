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
xml_vfind_pred_n(xmlnode* node, xml_pred_t* pred, int n, va_list args) {
  const void* vptr[4];
  int i;

  vptr[0] = vptr[1] = vptr[2] = vptr[3] = NULL;

  for(i = 0; i < n; ++i) {
    const void* arg = va_arg(args, const void*);

    if(i < 4) /* the predicate gets at most four arguments */
      vptr[i] = arg;
  }
  return xml_pfind_pred(node, pred, vptr);
}
