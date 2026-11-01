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
