#include "../strarray.h"
#include "../array.h"
#include "../str.h"

int
strarray_set(strarray* arr, size_t index, const char* s) {
  char** ptr = array_allocate(&arr->a, sizeof(char*), index);
  if(ptr) {
    *ptr = str_dup(s);
    return 1;
  }
  return 0;
}
