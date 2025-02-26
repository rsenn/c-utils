#include "../strarray.h"

int
strarray_from_vec(strarray* arr, char* vec[]) {
  int i;
  strarray_init(arr);

  for(i = 0; vec[i]; ++i)
    strarray_push(arr, vec[i]);
  return i;
}
