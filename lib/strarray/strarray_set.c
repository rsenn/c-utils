#include "../array.h"
#include "../str.h"
#include "../strarray.h"

int
strarray_set(strarray* arr, size_t index, const char* s) {
  char** ptr = array_allocate((array*)arr, sizeof(char*), index);
  if(ptr) {
    *ptr = str_dup(s);
    return 1;
  }
  return 0;
}

