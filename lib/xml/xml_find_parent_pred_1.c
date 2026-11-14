#include "../buffer.h"
#include "../str.h"
#include "../strlist.h"
#include "../xml_internal.h"

xmlnode*
xml_find_parent_pred_1(xmlnode* node, int (*pred)(xmlnode*, const void*, const void*, const void*), const void* arg) {
  const void* vptr[4];

  vptr[0] = arg;
  vptr[1] = NULL;
  vptr[2] = NULL;
  vptr[3] = NULL;

  return xml_pfind_parent(node, pred, vptr);
}
