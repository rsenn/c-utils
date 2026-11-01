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
xml_find_parent_attr(xmlnode* node, const char* attrs) {
  return xml_find_parent_pred_2(node, (xml_pred_t*)(void*)&xml_has_attr, NULL, attrs);
}
