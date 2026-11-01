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
xml_find_pred_1(xmlnode* node, xml_pred_t* pred, const void* arg) {
  const void* vptr[4];
  vptr[0] = arg;
  vptr[1] = NULL;
  vptr[2] = NULL;
  vptr[3] = NULL;
  return xml_pfind_pred(node, pred, vptr);
}
