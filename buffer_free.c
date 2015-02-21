#include <stdlib.h>
#if !(defined(_WIN32) || defined(__MINGW32__) ||defined(__MSYS__))
#include <sys/mman.h>
#else
#include <windows.h>
#endif
#include "buffer.h"

void buffer_free(buffer *b)
{
  switch(b->todo)
  {
    case FREE: free(b->x); break;
    case MUNMAP: 
#if defined(_WIN32) || defined(__MINGW32__) || defined(__MSYS__)
								 UnmapViewOfFile(b->x);
#else
							   munmap(b->x,b->a);
#endif
												break;
    default: ;
  }
  
  b->x = NULL;
  b->n = 0;
  b->p = 0;
  b->a = 0;
}


