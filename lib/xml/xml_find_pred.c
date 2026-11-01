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

xmlnode*
xml_find_pred_1(xmlnode* node, xml_pred_t* pred, const void* arg) {
  const void* vptr[4];
  vptr[0] = arg;
  vptr[1] = NULL;
  vptr[2] = NULL;
  vptr[3] = NULL;
  return xml_pfind_pred(node, pred, vptr);
}

xmlnode*
xml_find_pred_2(xmlnode* node, xml_pred_t* pred, const void* a0, const void* a1) {
  const void* vptr[4];
  vptr[0] = a0;
  vptr[1] = a1;
  vptr[2] = NULL;
  vptr[3] = NULL;
  return xml_pfind_pred(node, pred, vptr);
}
xmlnode*
xml_find_pred_3(xmlnode* node, xml_pred_t* pred, const void* a0, const void* a1, const void* a2) {
  const void* vptr[4];
  vptr[0] = a0;
  vptr[1] = a1;
  vptr[2] = a2;
  vptr[3] = NULL;
  return xml_pfind_pred(node, pred, vptr);
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

xmlnode*
xml_find_pred_n(xmlnode* node, xml_pred_t* pred, int n, ...) {
  xmlnode* ret;
  va_list args;
  va_start(args, n);

  ret = xml_vfind_pred_n(node, pred, n, args);

  va_end(args);

  return ret;
}
