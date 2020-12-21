#include "../alloc.h"

void
alloc_clear(void* ptr) {
	void** ptr_loc = (void**)ptr;
if(*ptr_loc) 
	alloc_free(*ptr_loc);
*ptr_loc=0;
}
