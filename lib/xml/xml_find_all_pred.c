#include <stdarg.h>
#include "../xml.h"
#include "../array.h"
#include "../byte.h"

static int
xml_find_all_predicate(xmlnode* node, array* a, int (*pred)(), void* arg1, void* arg2, void *arg3) {
  do {
    if(pred(node, arg1, arg2, arg3)) {
      xmlnode** nptr;
      if(!(nptr = array_allocate(a, sizeof(xmlnode*), array_length(a, sizeof(xmlnode*)))))
        return 0;
      *nptr = node;
    }
    if(node->children) {
     if(!xml_find_all_predicate(node->children, a, pred, arg1, arg2, arg3))
       return 0;
    }
  } while((node = node->next));
  return 1;
}

xmlnodeset
xml_find_all(xmlnode* node, int (*pred)(xmlnode*, void*), ...) {
  va_list args;
  va_start(args, pred);
  void* ptr[3];

  ptr[0] = va_arg(args, void*);
  ptr[1] = va_arg(args, void*);
  ptr[2] = va_arg(args, void*);

  xmlnodeset ret;
  array a;
  byte_zero(&a, sizeof(array));
  xml_find_all_predicate(node, &a, pred, ptr[0], ptr[1], ptr[2]);
  va_end(args);
  ret.nodes = array_start(&a);
  ret.count = array_length(&a,  sizeof(xmlnode*));
  return ret;
}

