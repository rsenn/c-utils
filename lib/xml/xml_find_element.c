#include "../buffer.h"
#include "../strlist.h"
#include "../xml.h"

int xml_has_attr(xmlnode* node, void* n, strlist* attrs);

int
xml_tag_pred(xmlnode* node, void* arg) {

//  stralloc p;
//  stralloc_init(&p);
//  xml_path(node, &p);
//  buffer_putsa(buffer_2, &p);
//  buffer_puts(buffer_2, " ");
//  buffer_putsa(buffer_2, arg);
//  buffer_putnlflush(buffer_2);

  return node->type == XML_ELEMENT && strlist_contains(arg, node->name);
}

xmlnode*
xml_find_element(xmlnode* node, const char* tag) {
  return xml_find_pred_1(node, &xml_tag_pred, tag);
}

xmlnode*
xml_find_attr(xmlnode* node, const char* attr) {
  return xml_find_pred_2(node, &xml_has_attr, NULL, attr);
}
