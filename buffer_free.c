#include <shell.h>
#ifndef __MINGW32__
#include <sys/mman.h>
#else
#include <windows.h>
#endif
#include "buffer.h"

void buffer_free(buffer *b)
{
  switch (b->todo)
  {
    case FREE: shell_free(b->x); break;
    case MUNMAP: 
#ifdef __MINGW32__
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


