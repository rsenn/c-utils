#include "../xml.h"
#include "../str.h"

int
xml_has_attribute(xmlnode* node, const char* attr) {
  return xml_has_attribute_n(node, attr, str_len(attr));
}
