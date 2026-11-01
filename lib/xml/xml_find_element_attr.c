#include "../strlist.h"
#include "../xml.h"
#include "../str.h"
/* The first element named tag whose attribute attr has the given value, looked for in
 * node, its following siblings and all their descendants. (The list based
 * predicate machinery of xml_find_pred hands strlists to its predicate, which a
 * predicate for plain strings read as garbage: this function never found anything.) */
/* the first element, in document order, that has one of the attributes in attrs */
/* attrs is a list of attribute names separated by '|' */

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
