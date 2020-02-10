#include "../xml.h"
#include "../str.h"
#include <stdarg.h>

HMAP_DB* xml_vattributes(const char* arg, va_list args);

xmlnode*
xml_child_element_attrs(const char* name, xmlnode* parent, const char* arg, ...) {
  va_list args;
  xmlnode* node = xml_child_element(name, parent);
  va_start(args, arg);
  node->attributes = xml_vattributes(arg, args);
  va_end(args);
  return node;
}
