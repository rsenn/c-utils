#include "../array.h"
#include "../byte.h"
#include "../strlist.h"
#include "../xml.h"
#include <stdarg.h>

static int
xml_find_all_predicate(xmlnode* node, array* a, int (*pred)(), void* arg1, void* arg2, void* arg3) {
  do {
    if(pred(node, arg1, arg2, arg3)) {
      xmlnode** nptr;
      if(!(nptr = array_allocate(a, sizeof(xmlnode*), array_length(a, sizeof(xmlnode*))))) return 0;
      *nptr = node;
    }
    if(node->children) {
      if(!xml_find_all_predicate(node->children, a, pred, arg1, arg2, arg3)) return 0;
    }
  } while((node = node->next));
  return 1;
}

xmlnodeset
xml_find_all(xmlnode* node, int (*pred)(), ...) {
  va_list args;
  va_start(args, pred);
  char* ptr[3];

  ptr[0] = va_arg(args, char*);
  ptr[1] = va_arg(args, char*);
  ptr[2] = va_arg(args, char*);

  xmlnodeset ret;
  array a;
  strlist names;

  strlist_init(&names);
  strlist_froms(&names, ptr[0], '|');

  byte_zero(&a, sizeof(array));
  xml_find_all_predicate(node, &a, pred, &names, ptr[1], ptr[2]);
  va_end(args);
  ret.nodes = array_start(&a);
  ret.size = array_length(&a, sizeof(xmlnode*));
  return ret;
}
