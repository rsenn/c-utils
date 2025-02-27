#include "../xml.h"
#include "../str.h"
#include <stdarg.h>

int
xml_vset_attributes(xmlnode* node, va_list args) {
  const char *name, *value;

  while((name = va_arg(args, const char*))) {
    if((value = va_arg(args, const char*)) == 0)
      return -1;

    xml_set_attribute(node, name, value);
  }
  return 0;
}

int
xml_set_attributes(xmlnode* node, ...) {
  int ret;
  va_list args;
  va_start(args, node);
  ret = xml_vset_attributes(node, args);
  va_end(args);
  return ret;
}
