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
xml_find_with_attrs(xmlnode* node, const char* attrs) {
  xmlnode* r;
  strlist attrlist;

  strlist_init(&attrlist, '\0');
  strlist_froms(&attrlist, attrs, '|');
  r = xml_find_with_attrs_l(node, &attrlist);
  strlist_free(&attrlist);
  return r;
}
