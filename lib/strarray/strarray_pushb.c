#include "../str.h"
#include "../strarray.h"

int
strarray_pushb(strarray* arr, const char* b, size_t n) {
  char** d = array_allocate((array*)arr,
                            sizeof(char*),
                            array_length((array*)arr, sizeof(char*)));

  return !!(*d = str_ndup(b, n));
}
