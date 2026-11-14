#include "../buffer.h"
#include "../str.h"
#include "../strlist.h"
#include "../xml_internal.h"

int xml_tag_pred(xmlnode* node, const void*, const void*, const void*);

xmlnode*
xml_find_parent(xmlnode* node, const char* tag) {
  return xml_find_parent_pred_1(node, &xml_tag_pred, tag);
}
