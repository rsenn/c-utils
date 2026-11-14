#include "../array.h"
#include "../byte.h"
#include "../strlist.h"
#include "../xml_internal.h"
#include <stdarg.h>

xmlnodeset
xml_find_all_3(xmlnode* node, xml_pred_t* pred, const void* a1, const void* a2, const void* a3) {
  const void* vptr[4];

  vptr[0] = a1;
  vptr[1] = a2;
  vptr[2] = a3;
  vptr[3] = NULL;

  return xml_pfind_all(node, pred, vptr);
}
