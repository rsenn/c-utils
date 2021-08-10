#include "../stralloc.h"
#include "../strlist.h"

void
strlist_copy(strlist* out, const strlist* in) {
  stralloc_copy(&out->sa, &in->sa);
  out->sep = in->sep;
}
