#include "../buffer.h"
#include "../str.h"
#include "../strlist.h"
#include "../xml_internal.h"

xmlnode*
xml_find_parent_attr(xmlnode* node, const char* attrs) {
  return xml_find_parent_pred_2(node, (xml_pred_t*)(void*)&xml_has_attr, NULL, attrs);
}
