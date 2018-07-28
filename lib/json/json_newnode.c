#include "../byte.h"
#include "../json.h"
#include <stdlib.h>

jsonval*
json_newnode(jsondata type) {
  jsonval* ret;
  if((ret = malloc(sizeof(jsonval)))) {
    byte_zero(ret, sizeof(jsonval));
    ret->type = type;
  }
  return ret;
}
