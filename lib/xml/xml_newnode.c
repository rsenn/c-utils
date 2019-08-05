#include "../alloc.h"
#include "../xml.h"

xmlnode*
xml_newnode(xmlnodeid type) {
  xmlnode* ret;

  if((ret = alloc_zero(sizeof(xmlnode))))
    ret->type = type;

  return ret;
}

