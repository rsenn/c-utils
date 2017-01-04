#include "strlist.h"
#include "byte.h"
#include "str.h"

int
strlist_pushsa(strlist* sl, const stralloc* sa) {
	return strlist_pushb(sl, sa->s, sa->len);
}
