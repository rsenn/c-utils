#include "../xml.h"

struct tag_attr_value {
  const char *t, *a, *v;
};

static int
xml_tag_attr_value_pred(xmlnode* node, void* ptr) {
  struct tag_attr_value* tav_ptr = ptr;
  if(node->type == XML_NODE_ELEMENT  && str_equal(tav_ptr->t, node->name)) {
    const char* a = xml_get_attribute(node, tav_ptr->a);
    if(a && str_equal(tav_ptr->v, a))
      return 1;
  }
  return 0;
}

xmlnode*
xml_find_element_attr(xmlnode* node, const char* tag, const char* attr, const char* value) {
 struct tag_attr_value tav = { tag, attr, value };
  return xml_find_pred(node, xml_tag_attr_value_pred, &tav);
}
