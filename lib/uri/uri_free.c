#include "../uri.h"
#include "../alloc.h"

void
uri_free(uri_t* uri) {
  alloc_clear(&uri->proto);
  alloc_clear(&uri->username);
  alloc_clear(&uri->password);
  alloc_clear(&uri->host);
  alloc_clear(&uri->location);
  alloc_clear(&uri->anchor);
  uri->port = 0;
}
