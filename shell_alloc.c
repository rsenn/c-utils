#include "shell.h"

#ifdef DEBUG
extern void *debug_alloc(const char *file, unsigned int line, size_t size);  

void *shell_allocdebug(const char *file, unsigned int line, size_t size)
{
  void *ptr = debug_alloc(file, line, size);
#else
void *shell_alloc(size_t size)
{
  void *ptr = malloc(size);
#endif /* DEBUG */
  
  /* exit if failed */
  if(ptr == NULL)
  {
    shell_error("malloc");
    exit(1);
  }  
  
  /* return pointer otherwise */
  return ptr;
}
