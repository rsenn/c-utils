#include "../byte.h"
#include "../stralloc.h"

size_t
stralloc_ends(const stralloc* sa, const stralloc* suffix) {
    return stralloc_endsb(sa, suffix->s, suffix->len);
}
