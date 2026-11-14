#include "../xml_internal.h"
#include "../strlist.h"
#include <stdarg.h>

xmlnode*
xml_find_pred_1(xmlnode* node, xml_pred_t* pred, const void* arg) {
  const void* vptr[4];

  vptr[0] = arg;
  vptr[1] = NULL;
  vptr[2] = NULL;
  vptr[3] = NULL;

  return xml_pfind_pred(node, pred, vptr);
}
