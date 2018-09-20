#include "../str.h"
#include "../strlist.h"
#include "../xml.h"

int
xml_match_name(xmlnode* node, strlist* names, const char* attr, const char* value) {
  (void)attr;
  (void)value;
  if(node->name == NULL) return 0;
  return strlist_contains(names, node->name);
}

int
xml_match_name_and_attr(xmlnode* node, strlist* names, const char* attr, const char* value) {
  if(node->name == NULL) return 0;
  if(!xml_has_attribute(node, attr)) return 0;

  if(strlist_contains(names, node->name)) {
    const char* a = xml_get_attribute(node, attr);
    if(a && str_equal(a, value)) return 1;
  }
  return 0;
}

int
xml_has_attr(xmlnode* node, strlist* names) {
  const char *str = strlist_at(names, 0), *e = strlist_end(names);
  if(node->name == NULL) return 0;

  while(str < e) {
    size_t n = byte_chr(str, e - str, names->sep);
    if(xml_has_attribute_n(node, str, n)) return 1;
    e += n;
  }
  return 0;
}
