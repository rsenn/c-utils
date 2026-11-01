#include "../buffer.h"
#include "../strlist.h"
#include "../xml.h"

xmlnode*
xml_find_attr(xmlnode* node, const char* attr) {
  return xml_find_pred_2(node, (xml_pred_t*)(void*)&xml_has_attr, NULL, attr);
}
