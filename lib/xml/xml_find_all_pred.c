#include "../array.h"
#include "../byte.h"
#include "../strlist.h"
#include "../xml.h"
#include <stdarg.h>

static int
xml_find_all_predicate(xmlnode* node,
                       array* a,
                       xml_pred_t* pred,
                       const void* vptr[]) {
  do {

    if(pred(node, vptr[0], vptr[1], vptr[2])) {
      xmlnode** nptr;
      if(!(nptr = (xmlnode**)array_allocate(
               a, sizeof(xmlnode*), array_length(a, sizeof(xmlnode*)))))
        return 0;
      *nptr = node;
    }

    if(node->children) {
      if(!xml_find_all_predicate(node->children, a, pred, vptr))
        return 0;
    }
  } while((node = node->next));
  return 1;
}

xmlnodeset
xml_pfind_all(xmlnode* node, xml_pred_t* pred, const void* ptr[4]) {
  xmlnodeset ret;
  array a;
  strlist names;
  array_init(&a);
  strlist_init(&names, '\0');
  if(ptr[0]) {
    strlist_froms(&names, (const char*)ptr[0], '|');
    byte_zero(&a, sizeof(array));
    ptr[0] = (const void*)&names;
  }
  xml_find_all_predicate(node, &a, pred, ptr);
  ret.nodes = (xmlnode**)array_start(&a);
  ret.size = array_length(&a, sizeof(xmlnode*));
  return ret;
}

xmlnodeset
xml_find_all_3(xmlnode* node,
               xml_pred_t* pred,
               const void* a1,
               const void* a2,
               const void* a3) {
  const void* vptr[4];
  vptr[0] = a1;
  vptr[1] = a2;
  vptr[2] = a3;
  vptr[3] = NULL;
  return xml_pfind_all(node, pred, vptr);
}

xmlnodeset
xml_find_all_2(xmlnode* node,
               xml_pred_t* pred,
               const void* a1,
               const void* a2) {
  const void* vptr[4];
  vptr[0] = a1;
  vptr[1] = a2;
  vptr[2] = NULL;
  vptr[3] = NULL;
  return xml_pfind_all(node, pred, vptr);
}

xmlnodeset
xml_find_all_1(xmlnode* node, xml_pred_t* pred, const void* a1) {
  const void* vptr[4];
  vptr[0] = a1;
  vptr[1] = NULL;
  vptr[2] = NULL;
  vptr[3] = NULL;
  return xml_pfind_all(node, pred, vptr);
}

// xmlnodeset
// xml_pfind_all(xmlnode* node, xml_pred_t* pred, ...) {
//  xmlnodeset ret;
//  va_list args;
//  va_start(args, pred);

//  ret = xml_vfind_all(node, pred, args);

//  va_end(args);

//  return ret;
//}
