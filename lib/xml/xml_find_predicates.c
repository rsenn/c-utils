#include "../str.h"
#include "../xml.h"

int
xml_match_name(xmlnode* node,
               const char* name,
               const char* attr,
               const char* value) {
  (void)attr;
  (void)value;

  char* ptr = NULL;
  char* s = str_tok((char*)name, "|", &ptr);

  do {

    if(str_equal(node->name, s)) return 1;

  } while((s = str_tok(NULL, "|", &ptr)));

  return 0;
}

int
xml_match_name_and_attr(xmlnode* node,
                        const char* name,
                        const char* attr,
                        const char* value) {
  if(str_equal(node->name, name)) {
    const char* a = xml_get_attribute(node, attr);
    if(a && str_equal(a, value)) return 1;
  }
  return 0;
}
