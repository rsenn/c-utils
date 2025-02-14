#include "../xml.h"
#include "../str.h"

xmlnode*
xml_attrnode(const char* name,
             size_t name_len,
             const char* value,
             size_t value_len) {
  xmlnode* attr;

  if((attr = xml_newnode(XML_ATTRIBUTE))) {
    attr->name = str_ndup(name, name_len);

    if((attr->children = xml_newnode(XML_TEXT))) {
      attr->children->parent = attr;
      attr->children->name = str_ndup(value, value_len);
    }
  }
  return attr;
}
