#include "../uri.h"
#include "../alloc.h"

void
uri_clear_anchor(uri_t* uri) {
  alloc_clear(&uri->anchor);
}
