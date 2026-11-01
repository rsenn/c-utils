#include "../strlist.h"
#include "../xml.h"
#include "../str.h"

int xml_has_attr(xmlnode* node, const void*, const strlist*);

typedef int(pred_fn_t)(xmlnode*, const void*, const void*, const void*);
/* The first element named tag whose attribute attr has the given value, looked for in
 * node, its following siblings and all their descendants. (The list based
 * predicate machinery of xml_find_pred hands strlists to its predicate, which a
 * predicate for plain strings read as garbage: this function never found anything.) */
xmlnode*
xml_find_element_attr(xmlnode* node, const char* tag, const char* attr, const char* value) {
  for(; node; node = node->next) {
    if(node->type == XML_ELEMENT && str_equal(tag, node->name)) {
      const char* a = xml_get_attribute(node, attr);

      if(a && str_equal(value, a))
        return node;
    }
    if(node->children) {
      xmlnode* r = xml_find_element_attr(node->children, tag, attr, value);

      if(r)
        return r;
    }
  }
  return NULL;
}

xmlnode*
xml_find_with_attrs_l(xmlnode* node, const strlist* attrs) {
  return xml_find_pred_1(node, (xml_pred_t*)(void*)&xml_has_attr, attrs);
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

  r = xml_find_all_2(node, (xml_pred_t*)(void*)&xml_has_attr, NULL, &attrlist);
  strlist_free(&attrlist);
  return r;
}
