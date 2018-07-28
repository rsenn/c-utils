#include "../byte.h"
#include "../json.h"
#include <stdlib.h>

jsonnode*
json_newnode(jsondata type) {
  jsonnode* ret;
  if((ret = malloc(sizeof(jsonnode)))) {
    byte_zero(ret, sizeof(jsonnode));
    ret->type = type;
  }
  return ret;
}
