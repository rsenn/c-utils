#include "../strlist.h"
#include "../xml.h"
#include "../buffer.h"

static int
xml_tag_pred(xmlnode* node, void* ptr[]) {
  stralloc p;
  stralloc_init(&p);
  xml_path(node, &p);
  buffer_putsa(buffer_2,&p);
  buffer_puts(buffer_2, " ");
  buffer_putsa(buffer_2, ptr);
  buffer_putnlflush(buffer_2);

  return node->type == XML_ELEMENT && strlist_contains(ptr[0], node->name);
}

xmlnode*
xml_find_element(xmlnode* node, const char* tag) {
  void* vptr[4];
  strlist names;
  strlist_init(&names, '\0');
  strlist_froms(&names, tag, '|');
  vptr[0] = &names;
  return xml_pfind_pred(node, &xml_tag_pred, vptr);
}
