#include "../str.h"
#include "../strlist.h"
#include "../xml.h"

int
xml_match_name(xmlnode* node, strlist* names, const char* attr, const char* value) {
  (void)attr;
  (void)value;
  if(node->name == NULL)
    return 0;
  return strlist_contains(names, node->name);
}

int
xml_match_name_and_attr(xmlnode* node, strlist* names, const char* attr, const char* value) {
  if(node->name == NULL)
    return 0;
  if(!xml_has_attribute(node, attr))
    return 0;

  if(strlist_contains(names, node->name)) {
    const char* a = xml_get_attribute(node, attr);
    if(a && str_equal(a, value))
      return 1;
  }
  return 0;
}

int
xml_has_attr(xmlnode* node, const void* n, strlist* attrs) {
  const char* str = strlist_at(attrs, 0);
  const char* e = strlist_end(attrs);

  if(node->name == NULL)
    return 0;

  while(str < e) {
    size_t n = byte_chr(str, e - str, attrs->sep);
    if(xml_has_attribute_n(node, str, n))
      return 1;
    str += n;
    if(str < e)
      ++str;
  }
  return 0;
}
