#include "../xml.h"

static int
xml_tag_pred(xmlnode* node, void* ptr) {
  return node->type == XML_NODE_ELEMENT && str_equal(ptr, node->name);
}

xmlnode*
xml_find_element(xmlnode* node, const char* tag) {
  return xml_find_pred(node, xml_tag_pred, tag);
}
