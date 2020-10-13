#include "../array.h"
#include "../str.h"
#include "../strarray.h"

int
strarray_setb(strarray* arr, size_t index, const char* s, size_t n) {
  char** ptr = (char**)array_allocate((array*)arr, sizeof(char*), index);

  if(ptr) {
    if(*ptr)
      free(*ptr);
    *ptr = str_ndup(s, n);
    return 1;
  }
  return 0;
}
