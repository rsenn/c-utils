#include "../strlist.h"
#include "../xml.h"
#include "../str.h"
/* The first element named tag whose attribute attr has the given value, looked for in
 * node, its following siblings and all their descendants. (The list based
 * predicate machinery of xml_find_pred hands strlists to its predicate, which a
 * predicate for plain strings read as garbage: this function never found anything.) */
/* the first element, in document order, that has one of the attributes in attrs */
/* attrs is a list of attribute names separated by '|' */

xmlnodeset
xml_find_all_attrs(xmlnode* node, const char* attrs) {
  return xml_find_all_2(node, (xml_pred_t*)(void*)&xml_has_attr, NULL, attrs);
}
