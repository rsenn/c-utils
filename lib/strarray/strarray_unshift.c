#include "../alloc.h"
#include "../array.h"
#include "../byte.h"
#include "../str.h"
#include "../strarray.h"

/* inserts a copy of s at the front; 1 on success */
int
strarray_unshift(strarray* arr, const char* s) {
  return strarray_splice(arr, 0, 0, 1, &s) != -1;
}
