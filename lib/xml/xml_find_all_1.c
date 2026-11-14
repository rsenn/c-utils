#include "../array.h"
#include "../byte.h"
#include "../strlist.h"
#include "../xml_internal.h"
#include <stdarg.h>

xmlnodeset
xml_find_all_1(xmlnode* node, xml_pred_t* pred, const void* a1) {
  const void* vptr[4];

  vptr[0] = a1;
  vptr[1] = NULL;
  vptr[2] = NULL;
  vptr[3] = NULL;

  return xml_pfind_all(node, pred, vptr);
}
