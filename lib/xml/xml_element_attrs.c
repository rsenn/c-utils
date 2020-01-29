#include "../xml.h"
#include "../str.h"
#include <stdarg.h>

xmlnode*
xml_element_attrs(const char* name, const char* arg, ...) {
	va_list args;
  xmlnode* node = xml_element(name);
  va_start(args, arg);
  node->attributes = xml_vattributes(arg, args);
  va_end(args);
  return node;
}
