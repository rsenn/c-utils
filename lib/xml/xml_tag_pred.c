#include "../buffer.h"
#include "../strlist.h"
#include "../xml.h"

int
xml_tag_pred(xmlnode* node, const void* arg, const void* arg2, const void* arg3) {
  return node && node->type == XML_ELEMENT && strlist_contains((strlist*)arg, node->name);
}
