#include "../xml_internal.h"
#include "../strlist.h"
#include <stdarg.h>

xmlnode*
xml_vfind_pred_n(xmlnode* node, xml_pred_t* pred, int n, va_list args) {
  const void* vptr[4];
  int i;

  vptr[0] = vptr[1] = vptr[2] = vptr[3] = NULL;

  for(i = 0; i < n; ++i) {
    const void* arg = va_arg(args, const void*);

    if(i < 4) /* the predicate gets at most four arguments */
      vptr[i] = arg;
  }

  return xml_pfind_pred(node, pred, vptr);
}
