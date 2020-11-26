#include "../ssl_internal.h"
#include "../buffer.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

const char*
ssl_instance_error(ssl_instance* i) {
  if(i->error)
    return ERR_reason_error_string(i->error);
  return "success";
}
#endif
