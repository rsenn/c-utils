#include "../xml.h"

double
xml_get_attribute_double(xmlnode* node, const char* attr) {

  const char* str; 
  
  if((str = xml_get_attribute(node, attr))) {
    double ret = 0;
    size_t n = scan_double(str, &ret);
    if(n > 0) return ret;
  }
  return NULL;
}
