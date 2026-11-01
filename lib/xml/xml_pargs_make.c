#include "../xml_internal.h"

void
xml_pargs_make(const void* ptr[], strlist lists[3]) {
  int i;

  for(i = 0; i < 3; ++i) {
    strlist_init(&lists[i], '\0');

    if(ptr[i]) {
      strlist_froms(&lists[i], (const char*)ptr[i], '|');
      ptr[i] = (const void*)&lists[i];
    }
  }
}
