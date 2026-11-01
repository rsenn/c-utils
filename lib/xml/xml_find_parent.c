#include "../buffer.h"
#include "../str.h"
#include "../strlist.h"
#include "../xml_internal.h"

int xml_tag_pred(xmlnode* node, const void*, const void*, const void*);

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
xml_find_parent(xmlnode* node, const char* tag) {
  return xml_find_parent_pred_1(node, &xml_tag_pred, tag);
}
