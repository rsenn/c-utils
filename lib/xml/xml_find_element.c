#include "../strlist.h"
#include "../xml.h"
static int
xml_tag_pred(xmlnode* node, void* ptr) {
  return node->type == XML_ELEMENT && strlist_contains(ptr, node->name);
}

xmlnode*
xml_find_element(xmlnode* node, const char* tag) {
  strlist names;
  strlist_froms(&names, tag, '|');
  return xml_find_pred(node, xml_tag_pred, &names);
}
