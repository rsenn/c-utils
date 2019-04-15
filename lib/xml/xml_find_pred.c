#include "../xml.h"
#include "../strlist.h"
#include <stdarg.h>

static xmlnode*
xml_find_predicate(xmlnode* node, int (*pred)(), const void* vptr[]) {
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
xml_pfind_pred(xmlnode* node, int (*pred)(/*xmlnode*,const void*,const void*,const void**/), const void* ptr[]) {
  xmlnode* ret;
  strlist names, attrs, values;
  strlist_init(&names, '\0');
  strlist_init(&attrs, '\0');
  strlist_init(&values, '\0');
  if(ptr[0]) {
    strlist_froms(&names, ptr[0], '|');
    ptr[0] = &names;
  }
  if(ptr[1]) {
    strlist_froms(&attrs, ptr[1], '|');
    ptr[1] = &attrs;
  }
  if(ptr[2]) {
    strlist_froms(&values, ptr[2], '|');
    ptr[2] = &values;
  }
  ret = xml_find_predicate(node, pred, ptr);
  if(names.sa.a)
    strlist_free(&names);
  if(attrs.sa.a)
    strlist_free(&attrs);
  if(values.sa.a)
    strlist_free(&values);
  return ret;
}

xmlnode*
xml_find_pred_1(xmlnode* node, int (*pred)(/*xmlnode*,const void*,*/), const void* arg) {
  const void* vptr[4];
  vptr[0] = arg;
  vptr[1] = NULL;
  vptr[2] = NULL;
  vptr[3] = NULL;
  return xml_pfind_pred(node, pred, vptr);
}

xmlnode*
xml_find_pred_2(xmlnode* node, int (*pred)(/*xmlnode*,const void*,const void**/), const void* a0, const void* a1) {
  const void* vptr[4];
  vptr[0] = a0;
  vptr[1] = a1;
  vptr[2] = NULL;
  vptr[3] = NULL;
  return xml_pfind_pred(node, pred, vptr);
}
xmlnode*
xml_find_pred_3(
    xmlnode* node, int (*pred)(/*xmlnode*,const void*,const void**/), const void* a0, const void* a1, const void* a2) {
  const void* vptr[4];
  vptr[0] = a0;
  vptr[1] = a1;
  vptr[2] = a2;
  vptr[3] = NULL;
  return xml_pfind_pred(node, pred, vptr);
}

xmlnode*
xml_vfind_pred_n(xmlnode* node, int (*pred)(xmlnode*, const void*), int n, va_list args) {
  const void* vptr[4];
  int i;
  for(i = 0; i < n; ++i) {
    vptr[i] = va_arg(args, const void*);
  }
  return xml_pfind_pred(node, pred, vptr);
}

xmlnode*
xml_find_pred_n(xmlnode* node, int (*pred)(xmlnode*, const void*), int n, ...) {
  xmlnode* ret;
  va_list args;
  va_start(args, n);

  ret = xml_vfind_pred_n(node, pred, n, args);

  va_end(args);

  return ret;
}
