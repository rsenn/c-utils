#include "../buffer.h"
#include "../str.h"
#include "../strlist.h"
#include "../xml_internal.h"

xmlnode*
xml_find_parent_pred_3(xmlnode* node, xml_pred_t* pred, const void* a0, const void* a1, const void* a2) {
  const void* vptr[4];

  vptr[0] = a0;
  vptr[1] = a1;
  vptr[2] = a2;
  vptr[3] = NULL;

  return xml_pfind_parent(node, (xml_pred_t*)pred, vptr);
}
