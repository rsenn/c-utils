#include "../buffer.h"
#include "../str.h"
#include "../strlist.h"
#include "../xml_internal.h"

static xmlnode*
xml_find_parent_predicate(xmlnode* node,
                          int (*pred)(xmlnode*, const void*, const void*, const void*),
                          const void* vptr[]) {
  while((node = node->parent)) {
    if(pred(node, vptr[0], vptr[1], vptr[2]))
      break;
  }
  return node;
}

xmlnode*
xml_find_parent_pred_2(xmlnode* node,
                       int (*pred)(xmlnode*, const void*, const void*, const void*),
                       const void* a0,
                       const void* a1) {
  const void* vptr[4];
  vptr[0] = a0;
  vptr[1] = a1;
  vptr[2] = NULL;
  vptr[3] = NULL;
  return xml_pfind_parent(node, pred, vptr);
}
