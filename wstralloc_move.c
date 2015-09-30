#include <stdlib.h>
#include "byte.h"
#include "wstralloc.h"

void wstralloc_move(wstralloc* to, wstralloc* from) 
{
  if(to->a == 0) to->s = NULL;
  wstralloc_free(to);
  to->s = from->s;
  to->a = from->a;
  to->len = from->len;
}
