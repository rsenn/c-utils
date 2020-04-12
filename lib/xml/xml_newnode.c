#include "../alloc.h"
#include "../xml.h"

xmlnode*
xml_newnode(xmlnodeid type) {
  xmlnode* ret;

  if((ret = (xmlnode*)alloc_zero(sizeof(xmlnode)))) {
    ret->type = type;
    ret->attributes = NULL;
    ret->children = NULL;
    ret->next = NULL;
  }

  return ret;
}
