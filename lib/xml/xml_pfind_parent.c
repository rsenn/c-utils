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
xml_pfind_parent(xmlnode* node, int (*pred)(xmlnode*, const void*, const void*, const void*), const void* ptr[]) {
  xmlnode* ret;
  strlist lists[3];

  xml_pargs_make(ptr, lists);
  ret = (xmlnode*)xml_find_parent_predicate(node, pred, ptr);
  xml_pargs_free(lists);
  return ret;
}
