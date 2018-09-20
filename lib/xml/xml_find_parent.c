#include "../buffer.h"
#include "../strlist.h"
#include "../xml.h"
int
xml_tag_pred(xmlnode* node, void* arg);
static xmlnode*
xml_find_parent_predicate(xmlnode* node, int (*pred)(), void* vptr[]) {
  while((node = node->parent)) {
    if(pred(node, vptr[0], vptr[1], vptr[2]))
      break;
  }
  return node;
}

xmlnode*
xml_pfind_parent(xmlnode* node, int (*pred)(), void* ptr[4]) {
  xmlnode* ret;
  strlist names;
  strlist_init(&names, '\0');
  if(ptr[0]) {
    strlist_froms(&names, ptr[0], '|');
    ptr[0] = &names;
  }
  ret = xml_find_parent_predicate(node, pred, ptr);
  if(names.sa.a)
    strlist_free(&names);
  return ret;
}

xmlnode*
xml_find_parent(xmlnode* node, const char* tag) {
  void* vptr[4] = {0, 0, 0, 0};
  vptr[0] = tag;
  return xml_pfind_parent(node, &xml_tag_pred, vptr);
}
