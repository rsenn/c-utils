#include "../array.h"
#include "../byte.h"
#include "../strlist.h"
#include "../xml.h"
#include <stdarg.h>

static int
xml_find_all_predicate(xmlnode* node, array* a, int (*pred)(), void* vptr[]) {
  do {

    if(pred(node, vptr[0], vptr[1], vptr[2])) {
      xmlnode** nptr;
      if(!(nptr = array_allocate(a, sizeof(xmlnode*), array_length(a, sizeof(xmlnode*))))) return 0;
      *nptr = node;
    }

    if(node->children) {
      if(!xml_find_all_predicate(node->children, a, pred, vptr)) return 0;
    }
  } while((node = node->next));
  return 1;
}

xmlnodeset
xml_pfind_all(xmlnode* node, int (*pred)(), void* ptr[]) {
   xmlnodeset ret;
  array a;
  strlist names; 

  strlist_init(&names, '\0');
  strlist_froms(&names, ptr[0], '|');
  byte_zero(&a, sizeof(array));
 ptr[0] = &names;
  xml_find_all_predicate(node, &a, pred, ptr);
  ret.nodes = array_start(&a);
  ret.size = array_length(&a, sizeof(xmlnode*));
  return ret;
}

xmlnodeset
xml_find_all_3(xmlnode* node, int (*pred)(), void* a1, void* a2, void* a3) {
 void* vptr[] = { a1, a2, a3, NULL};
  return xml_pfind_all(node, pred, vptr);
}

xmlnodeset
xml_find_all_2(xmlnode* node, int (*pred)(), void* a1, void* a2) {
 void* vptr[] = { a1, a2, NULL, NULL};
  return xml_pfind_all(node, pred, vptr);
}

xmlnodeset
xml_find_all_1(xmlnode* node, int (*pred)(), void* a1) {
 void* vptr[] = { a1, NULL, NULL, NULL};
  return xml_pfind_all(node, pred, vptr);
}


//xmlnodeset
//xml_pfind_all(xmlnode* node, int (*pred)(), ...) {
//  xmlnodeset ret;
//  va_list args;
//  va_start(args, pred);

//  ret = xml_vfind_all(node, pred, args);

//  va_end(args);

//  return ret;
//}
