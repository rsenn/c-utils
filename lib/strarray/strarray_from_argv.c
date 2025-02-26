#include "../strarray.h"

int
strarray_from_argv(int argc, const char* const argv[], strarray* arr) {
  int i;
  strarray_init(arr);

  for(i = 0; i < argc; ++i) {
    strarray_push(arr, argv[i]);
  }
  return i;
}
