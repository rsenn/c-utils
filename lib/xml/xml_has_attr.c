#include "../str.h"
#include "../strlist.h"
#include "../xml.h"

int
xml_has_attr(xmlnode* node, const void* n, const strlist* attrs) {
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
