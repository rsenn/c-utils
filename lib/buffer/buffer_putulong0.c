#include "../buffer.h"
#include "../fmt.h"

int
buffer_putulong0(buffer* b, unsigned long l, int pad) {

#ifdef HAVE_ALLOCA
	char* buf = alloca(FMT_ULONG + pad);
#elif defined(HAVE_DYNSTACK)
  char buf[FMT_ULONG+pad];
#else
	char* buf = malloc(FMT_ULONG + pad);
#endif
  int ret = buffer_put(b, buf, fmt_ulong0(buf, l, pad));

#if !defined(HAVE_ALLOCA) && !defined(HAVE_DYNSTACK)
  free(buf);
#endif
  return ret;
}
