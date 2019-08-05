#if 1 // ndef HAVE_PIPE2

#include "../windoze.h"
#include "../io_internal.h"
#include "../ndelay.h"
#include <errno.h>
#include <fcntl.h>

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

#ifndef O_CLOEXEC
#define O_CLOEXEC 0
#endif

#ifndef O_NONBLOCK
#define O_NONBLOCK 0
#endif

#if defined(__LCC__) && defined(_WIN64)
int pipe(int*, unsigned int, int);
#endif

#if defined(__MINGW32__) || defined(__MINGW64__) || defined(_MSC_VER) || defined(__BORLANDC__)
#define pipe _pipe
#endif

#ifdef __DMC__
int
pipe(fd_t fd[2]) {
  HANDLE phandles[2];

  /* To get non-blocking pipes we could use CreateNamedPipe here. But it isn't
      implemented under Win9x. */
  if(!CreatePipe((HANDLE*)&phandles[0], (HANDLE*)&phandles[1], NULL, 0)) {
    errno = EMFILE;
    return -1;
  } else {
    errno = 0;
    fd[0] = _open_osfhandle((DWORD)phandles[0], O_RDONLY|O_BINARY);
    fd[1] = _open_osfhandle((DWORD)phandles[1], O_WRONLY|O_BINARY);
    return 0;
  }
}
#endif

int
pipe2(int fd[2], int flags) {
  /* Mingw _pipe() corrupts fd on failure; also, if we succeed at
     creating the pipe but later fail at changing fcntl, we want
     to leave fd unchanged: http://austingroupbugs.net/view.php?id=467  */
  int tmp[2];
  tmp[0] = fd[0];
  tmp[1] = fd[1];

  /* Check the supported flags.  */
  if((flags & ~(O_CLOEXEC
#ifdef O_BINARY
                | O_BINARY
#endif

#ifdef O_TEXT
                | O_TEXT
#endif
                )) != 0) {
    errno = EINVAL;
    return -1;
  }

#if defined(_WIN32) && !(defined(__CYGWIN__) || defined(__MSYS__)) && !defined(__DMC__)
  /* Native Windows API.  */

  if(pipe(fd, 4096, flags & ~O_NONBLOCK) < 0) {
    fd[0] = tmp[0];
    fd[1] = tmp[1];
    return -1;
  }

  /* O_NONBLOCK handling.
     On native Windows platforms, O_NONBLOCK is defined by gnulib.  Use the
     functions defined by the gnulib module 'nonblocking'.  */
#ifndef PIPE2_NDELAY_OFF
  if(flags & O_NONBLOCK) {
    if(ndelay_on(fd[0]) != 0 || ndelay_on(fd[1]) != 0)
      goto fail;
  }
/*#else
   { verify(O_NONBLOCK == 0); }*/
#endif

  return 0;

#else
  /* Unix API.  */

  if(pipe(fd) < 0)
    return -1;

  /* POSIX <http://www.opengroup.org/onlinepubs/9699919799/functions/pipe.html>
     says that initially, the O_NONBLOCK and FD_CLOEXEC flags are cleared on
     both fd[0] and fd[1].  */

  /* O_NONBLOCK handling.
     On Unix platforms, O_NONBLOCK is defined by the system.  Use fcntl().  */
 #ifdef F_SETFL
  if(flags & O_NONBLOCK) {
    int fcntl_flags;

    if((fcntl_flags = fcntl(fd[1], F_GETFL, 0)) < 0 || fcntl(fd[1], F_SETFL, fcntl_flags | O_NONBLOCK) == -1 ||
       (fcntl_flags = fcntl(fd[0], F_GETFL, 0)) < 0 || fcntl(fd[0], F_SETFL, fcntl_flags | O_NONBLOCK) == -1)
      goto fail;
  }

  if(flags & O_CLOEXEC) {
    int fcntl_flags;

    if((fcntl_flags = fcntl(fd[1], F_GETFD, 0)) < 0 || fcntl(fd[1], F_SETFD, fcntl_flags | FD_CLOEXEC) == -1 ||
       (fcntl_flags = fcntl(fd[0], F_GETFD, 0)) < 0 || fcntl(fd[0], F_SETFD, fcntl_flags | FD_CLOEXEC) == -1)
      goto fail;
  }
#endif

#if 0 //def O_BINARY
  if(flags & O_BINARY) {
    set_binary_mode(fd[1], O_BINARY);
    set_binary_mode(fd[0], O_BINARY);
  } else if(flags & O_TEXT) {
    set_binary_mode(fd[1], O_TEXT);
    set_binary_mode(fd[0], O_TEXT);
  }
#endif

  return 0;

#endif

#if !defined(NDELAY_OFF) || !(defined _WIN32 && !defined __CYGWIN__)
fail : {
  int saved_errno = errno;
  close(fd[0]);
  close(fd[1]);
  fd[0] = tmp[0];
  fd[1] = tmp[1];
  errno = saved_errno;
  return -1;
}
#endif
}

#endif /* HAVE_PIPE2 */

