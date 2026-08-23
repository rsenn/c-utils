#include "../io_internal.h"

/* Query whether io_nonblock()/io_block() last put d in non-blocking mode.
 * This is a self-tracked cache (io_entry::nonblock), not a kernel query --
 * on WINDOWS_NATIVE, Winsock's ioctlsocket(FIONBIO) is set-only, there is
 * no documented way to read a socket's current blocking mode back from the
 * OS. Accurate as long as d's mode was last changed through io_nonblock()/
 * io_block() (or io_fd()/io_fd_flags() at registration); an external
 * fcntl()/ioctlsocket() call bypassing those won't be reflected here. */
int
io_isnonblock(fd_type d) {
  io_entry* e = io_getentry(d);

  if(!e)
    return -1;

  return e->nonblock;
}
