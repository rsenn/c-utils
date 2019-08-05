#include "../alloc.h"
#include "../byte.h"
#include "../json.h"
#include <stdlib.h>

jsonval*
json_newnode(jsondata type) {
  jsonval* ret;
  if((ret = alloc_zero(sizeof(jsonval)))) {
    ret->type = type;
  }
  return ret;
}

