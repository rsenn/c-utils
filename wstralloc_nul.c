#include "wstralloc.h"

int wstralloc_nul(wstralloc* sa)
{
  if(wstralloc_ready(sa, sa->len + 1)) 
  {
    sa->s[sa->len]='\0';
    return 1;
  }
  return 0;
}
