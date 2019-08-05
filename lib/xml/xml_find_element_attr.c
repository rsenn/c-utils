#include "../strlist.h"
#include "../xml.h"
#include "../str.h"

extern int xml_has_attr(xmlnode* node, strlist* names);

struct tag_attr_value {
  const char *t, *a, *v;
};

static int
xml_tag_attr_value_pred(xmlnode* node, const char* tag, const char* attr, const char* value) {

  if(node->type == XML_ELEMENT && str_equal(tag, node->name)) {
    const char* a = xml_get_attribute(node, attr);
    if(a && str_equal(value, a))
      return 1;
  }
  return 0;
}

xmlnode*
xml_find_element_attr(xmlnode* node, const char* tag, const char* attr, const char* value) {
  return xml_find_pred_3(node, xml_tag_attr_value_pred, tag, attr, value);
}

xmlnode*
xml_find_with_attrs_l(xmlnode* node, const strlist* attrs) {
  return xml_find_pred_1(node, xml_has_attr, attrs);
}

xmlnode*
xml_find_with_attrs(xmlnode* node, const char* attrs) {
  xmlnode* r;
  strlist attrlist;
  strlist_init(&attrlist, '|');
  strlist_froms(&attrlist, attrs, '|');
  r = xml_find_with_attrs_l(node, &attrlist);
  strlist_free(&attrlist);
  return r;
}

xmlnodeset
xml_find_all_attrs(xmlnode* node, const char* attrs) {
  xmlnodeset r;
  strlist attrlist;
  xmlnodeset_clear(&r);
  strlist_init(&attrlist, '|');
  // strlist_froms(&attrlist, attrs, '|');
  stralloc_copys(&attrlist.sa, attrs);

  r = xml_find_all_2(node, xml_has_attr, NULL, &attrlist);
  strlist_free(&attrlist);
  return r;
}

