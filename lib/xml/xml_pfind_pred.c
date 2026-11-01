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
xml_pfind_pred(xmlnode* node, xml_pred_t* pred, const void* ptr[]) {
  xmlnode* ret;
  strlist lists[3];

  xml_pargs_make(ptr, lists);
  ret = xml_find_predicate(node, pred, ptr);
  xml_pargs_free(lists);
  return ret;
}
