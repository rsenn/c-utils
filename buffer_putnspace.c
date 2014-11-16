#include "config.h"

#include <stdlib.h>

#ifdef HAVE_ALLOCA_H
#include <alloca.h>
#endif /* HAVE_ALLOCA_H */

#include "str.h"
#include "byte.h"
#include "buffer.h"

int buffer_putnspace(buffer* b, int n) {
  if(n <= 0) return 0;

	{ 
    int ret;
    char *space = 
#ifdef HAVE_ALLOCA_FUNC
      alloca(n);
#else
      malloc(n);
#endif
    byte_fill(space, n, ' ');
    ret = buffer_put(b,space,n);
#ifndef HAVE_ALLOCA_FUNC
    free(space);
#endif
    return ret;
	}
}
