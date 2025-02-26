#include "../stralloc.h"
#include "../strlist.h"

void
strlist_copy(strlist* out, const strlist* in) {
  size_t i;
  stralloc_copy(&out->sa, &in->sa);

  for(i = 0; i < out->sa.len; i++) {
    if(out->sa.s[i] == in->sep)
      out->sa.s[i] = out->sep;
  }
  //  out->sep = in->sep;
}
