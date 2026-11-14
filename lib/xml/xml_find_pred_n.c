#include "../xml_internal.h"
#include "../strlist.h"
#include <stdarg.h>

xmlnode*
xml_find_pred_n(xmlnode* node, xml_pred_t* pred, int n, ...) {
  xmlnode* ret;
  va_list args;
  va_start(args, n);

  ret = xml_vfind_pred_n(node, pred, n, args);
  va_end(args);
  return ret;
}
