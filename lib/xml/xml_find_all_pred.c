#include "../array.h"
#include "../byte.h"
#include "../strlist.h"
#include "../xml.h"
#include <stdarg.h>

static int
xml_find_all_predicate(xmlnode* node, array* a, int (*pred)(), const void* arg1, const void* arg2, const void* arg3, const void* arg4) {
  do {

    if(pred(node, arg1, arg2, arg3)) {
      xmlnode** nptr;
      if(!(nptr = array_allocate(a, sizeof(xmlnode*), array_length(a, sizeof(xmlnode*))))) return 0;
      *nptr = node;
    }

    if(node->children) {
      if(!xml_find_all_predicate(node->children, a, pred, arg1, arg2, arg3, arg4)) return 0;
    }
  } while((node = node->next));
  return 1;
}

xmlnodeset
xml_find_all(xmlnode* node, int (*pred)(), const void* ptr[4]) {
   xmlnodeset ret;
  array a;
  strlist names; 

  strlist_init(&names, '\0');
  strlist_froms(&names, ptr[0], '|');
  byte_zero(&a, sizeof(array));
  xml_find_all_predicate(node, &a, pred, &names, ptr[1], ptr[2], ptr[3]);
  ret.nodes = array_start(&a);
  ret.size = array_length(&a, sizeof(xmlnode*));
  return ret;
}
