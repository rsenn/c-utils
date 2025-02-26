#include "../tls_internal.h"
#include "../str.h"
#include <errno.h>

#if WINDOWS_NATIVE
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#endif

#ifdef HAVE_OPENSSL
#include <openssl/bio.h>

tls_bio_t*
tls_bio_socket(fd_type fd) {

  tls_bio_t* bio;

  bio = BIO_new_socket(fd, 0);

  /*

    if((bio = BIO_new((void*)tls_bio_method())))
      BIO_set_fd(bio, fd, 0);*/

  return bio;
}
#endif
