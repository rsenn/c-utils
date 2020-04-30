#include "../buffer.h"
#include "../strlist.h"
#include "../xml.h"

int xml_has_attr(xmlnode* node, const void*, const void*, const void*);
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
xml_pfind_parent(xmlnode* node, int (*pred)(xmlnode*, const void*, const void*, const void*), const void* ptr[]) {
  xmlnode* ret;
  strlist names, attrs;
  strlist_init(&names, '\0');
  strlist_init(&attrs, '\0');
  if(ptr[0]) {
    strlist_froms(&names, (const char*)ptr[0], '|');
    ptr[0] = (const void*)&names;
  }
  if(ptr[1]) {
    strlist_froms(&attrs, (const char*)ptr[1], '|');
    ptr[1] = (const void*)&attrs;
  }
  ret = (xmlnode*)xml_find_parent_predicate(node, pred, ptr);
  if(names.sa.a)
    strlist_free(&names);
  if(attrs.sa.a)
    strlist_free(&attrs);
  return ret;
}

xmlnode*
xml_find_parent_pred_1(xmlnode* node, int (*pred)(xmlnode*, const void*, const void*, const void*), const void* arg) {
  const void* vptr[4];
  vptr[0] = arg;
  vptr[1] = NULL;
  vptr[2] = NULL;
  vptr[3] = NULL;
  return xml_pfind_parent(node, pred, vptr);
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
xmlnode*
xml_find_parent_pred_3(xmlnode* node, xml_pred_t* pred, const void* a0, const void* a1, const void* a2) {
  const void* vptr[4];
  vptr[0] = a0;
  vptr[1] = a1;
  vptr[2] = a2;
  vptr[3] = NULL;
  return xml_pfind_parent(node, (xml_pred_t*)pred, vptr);
}

xmlnode*
xml_find_parent(xmlnode* node, const char* tag) {
  return xml_find_parent_pred_1(node, &xml_tag_pred, tag);
}

xmlnode*
xml_find_parent_attr(xmlnode* node, const char* attrs) {
  return xml_find_parent_pred_2(node, &xml_has_attr, NULL, attrs);
}
