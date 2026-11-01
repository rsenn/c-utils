#include "../array.h"
#include "../byte.h"
#include "../strlist.h"
#include "../xml_internal.h"
#include <stdarg.h>
// xmlnodeset
// xml_pfind_all(xmlnode* node, xml_pred_t* pred, ...) {
//  xmlnodeset ret;
//  va_list args;
//  va_start(args, pred);
//  ret = xml_vfind_all(node, pred, args);
//  va_end(args);
//  return ret;
//}

static int
xml_find_all_predicate(xmlnode* node, array* a, xml_pred_t* pred, const void* vptr[]) {
  if(node == NULL)
    return 1;

  do {

    if(pred(node, vptr[0], vptr[1], vptr[2])) {
      xmlnode** nptr;

      if(!(nptr = (xmlnode**)array_allocate(a, sizeof(xmlnode*), array_length(a, sizeof(xmlnode*)))))
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
  strlist lists[3];

  byte_zero(&a, sizeof(array));
  xml_pargs_make(ptr, lists);
  xml_find_all_predicate(node, &a, pred, ptr);
  xml_pargs_free(lists);
  ret.nodes = (xmlnode**)array_start(&a);
  ret.size = array_length(&a, sizeof(xmlnode*));
  return ret;
}
