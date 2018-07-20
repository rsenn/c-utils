#include "../byte.h"
#include "../xml.h"
#include <stdlib.h>

xmlnode*
xml_newnode(xmlnodeid type) {
  xmlnode* ret;
  if((ret = malloc(sizeof(xmlnode)))) {
    byte_zero(ret, sizeof(xmlnode));
    ret->type = type;
  }
  return ret;
}
