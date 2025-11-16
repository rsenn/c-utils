#include "../tls_internal.h"
#include "../io.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
void
tls_io(fd_type fd) {
  ssize_t ret;
  tls_instance_t* i = iarray_get(&tls_list, fd);
  assert(i);

  i->wantread = &io_onlywantread;
  i->wantwrite = &io_onlywantwrite;
}
#endif
