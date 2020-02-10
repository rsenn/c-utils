#include "../http.h"
#include "../windoze.h"
#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif
void
http_close(http* h) {
  if(h->sock != -1)
    close(h->sock);
  h->sock = -1;
}
