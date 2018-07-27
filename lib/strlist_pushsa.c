#include "strlist.h"
#include "byte.h"
#include "str.h"

int
strlist_pushsa(strlist* sl, const stralloc* sa) {
    stralloc_cat((stralloc*)sl, sa);
    return stralloc_catb((stralloc*)sl, "\0", 1);
}
