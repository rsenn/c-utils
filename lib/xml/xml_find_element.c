#include "../buffer.h"
#include "../strlist.h"
#include "../xml.h"

int xml_has_attr(xmlnode* node, const void* n, strlist* attrs);

int
xml_tag_pred(xmlnode* node, const void* arg, const void* arg2, const void* arg3) {

  //  stralloc p;
  //  stralloc_init(&p);
  //  xml_path(node, &p);
  //  buffer_putsa(buffer_2, &p);
  //  buffer_puts(buffer_2, " ");
  //  buffer_putsa(buffer_2, arg);
  //  buffer_putnlflush(buffer_2);

  return node && node->type == XML_ELEMENT && strlist_contains((strlist*)arg, node->name);
}

xmlnode*
xml_find_element(xmlnode* node, const char* tag) {
  return xml_find_pred_1(node, &xml_tag_pred, tag);
}

xmlnode*
xml_find_attr(xmlnode* node, const char* attr) {
  return xml_find_pred_2(node, (xml_pred_t*)&xml_has_attr, NULL, attr);
}
