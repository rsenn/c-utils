#include "../strlist.h"
#include "../xml.h"
#include "../buffer.h"

static int
xml_tag_pred(xmlnode* node, void* ptr) {
  stralloc p;
  stralloc_init(&p);
  xml_path(node, &p);
  buffer_putsa(buffer_2,&p);
  buffer_puts(buffer_2, " ");
  buffer_putsa(buffer_2, ptr);
  buffer_putnlflush(buffer_2);

  return node->type == XML_ELEMENT && strlist_contains(ptr, node->name);
}

xmlnode*
xml_find_element(xmlnode* node, const char* tag) {
  strlist names;
  strlist_init(&names);
  strlist_froms(&names, tag, '|');
  return xml_find_pred(node, &xml_tag_pred, &names);
}
