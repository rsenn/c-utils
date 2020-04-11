#include "../array.h"
#include "../str.h"
#include "../strarray.h"

int
strarray_push_unique(strarray* arr, const char* s) {
  if(strarray_index_of(arr, s) == -1)
    return strarray_push(arr, s);
  return 0;
}
