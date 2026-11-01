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
