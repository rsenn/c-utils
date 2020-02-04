#include "../array.h"
#include "../str.h"
#include "../strarray.h"

int
strarray_setb(strarray* arr, size_t index, const char* s, size_t n) {
  char** ptr = array_allocate((array*)arr, sizeof(char*), index);
  if(ptr) {
    *ptr = str_ndup(s, n);
    return 1;
  }
  return 0;
}
