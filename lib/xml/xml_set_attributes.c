#include "../xml.h"
#include "../str.h"
#include <stdarg.h>

int
xml_set_attributes(xmlnode* node, ...) {
  int ret;
  va_list args;
  va_start(args, node);
  ret = xml_vset_attributes(node, args);
  va_end(args);
  return ret;
}
