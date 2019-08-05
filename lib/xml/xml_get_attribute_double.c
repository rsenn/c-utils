#include "../xml.h"
#include "../scan.h"

double
xml_get_attribute_double(xmlnode* node, const char* attr) {
  const char* str;

  if((str = xml_get_attribute(node, attr))) {
    double ret = 0;
    size_t n;
    if((n = scan_double(str, &ret)))
      return ret;
  }
  return 0;
}

