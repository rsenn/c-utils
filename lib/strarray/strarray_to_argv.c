#include "../byte.h"
#include "../strarray.h"
#include <stdlib.h>

char**
strarray_to_argv(strarray* arr) {
  char** av;
  size_t n = strarray_size(arr);
  if((av = calloc(n + 1, sizeof(char*)))) {
    byte_copy(av, sizeof(char*) * n, array_start(&arr->a));
  }
  return av;
}
