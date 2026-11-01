#include "../xml_internal.h"

void
xml_pargs_free(strlist lists[3]) {
  int i;

  for(i = 0; i < 3; ++i)
    if(lists[i].sa.a)
      strlist_free(&lists[i]);
}
