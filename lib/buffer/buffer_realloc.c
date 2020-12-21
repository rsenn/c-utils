#include "../buffer.h"
#include "../alloc.h"

void
buffer_realloc(buffer* b,   size_t ylen) {
	if(b->deinit == &buffer_free)
		b->deinit(b);
   b->x = alloc(ylen);
  b->a = ylen; 
  b->deinit = &buffer_free;
}
