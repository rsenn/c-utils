#include "../xml.h"
#include "../str.h"

int
xml_match_name(xmlnode* node, const char* name) {
  return str_equal(node->name, name);
}

int
xml_match_name_and_attr(xmlnode* node, const char* name, const char* attr, const char* value) {
  if(str_equal(node->name, name)) {
    const char* a = xml_get_attribute(node, attr);
    if(a && str_equal(a, value))
      return 1; 
  }
  return 0;
}
