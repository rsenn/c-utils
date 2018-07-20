#include <stdlib.h>
#include "../xml.h"
#include "../byte.h"

xmlnode*
xml_newnode(xmlnodeid type) {
    xmlnode* ret;

    if((ret = malloc(sizeof(xmlnode)))) {
        byte_zero(ret, sizeof(xmlnode));
        ret->type = type;
    }

    return ret;
}
