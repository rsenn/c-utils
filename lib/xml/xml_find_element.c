#include "../buffer.h"
#include "../strlist.h"
#include "../xml.h"

xmlnode*
xml_find_element(xmlnode* node, const char* tag) {
  return xml_find_pred_1(node, &xml_tag_pred, tag);
}
