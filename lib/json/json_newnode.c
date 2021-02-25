#include "../json_internal.h"
#include "../alloc.h"
#include "../byte.h"
#include <stdlib.h>

jsonval*
json_newnode(jsontype type) {
  jsonval* ret;
  if((ret = alloc_zero(sizeof(jsonval)))) {
    ret->type = type;
  }
  return ret;
}
