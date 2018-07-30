#include <stdlib.h>
#include "../buffer.h"
#include "../fmt.h"

int
buffer_putxlong0(buffer* b, unsigned long l, int pad) {

#ifdef HAVE_ALLOCA
	char* buf = alloca(FMT_XLONG + pad);
#elif defined(HAVE_DYNSTACK)
  char buf[FMT_XLONG+pad];
#else	
	char* buf = malloc(FMT_XLONG + pad);
#endif
  int ret = buffer_put(b, buf, fmt_xlong0(buf, l, pad));

#if !defined(HAVE_ALLOCA) && !defined(HAVE_DYNSTACK)
  free(buf);
#endif
  return ret;
}
