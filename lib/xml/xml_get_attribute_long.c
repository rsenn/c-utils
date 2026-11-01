#include "../xml.h"
#include "../scan.h"

long
xml_get_attribute_long(xmlnode* node, const char* attr) {
  const char* str;

  if((str = xml_get_attribute(node, attr))) {
    long ret = ~0;
    size_t n;

    if((n = scan_long(str, &ret)))
      return ret;
  }
  return 0;
}
