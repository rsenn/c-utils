#include <sys/param.h>
#include <sys/types.h>
#if !defined(_WIN32) && !defined(_WIN64)
#include <netinet/in.h>
#include <sys/socket.h>
#endif
#include "../socket.h"
#include "../windoze.h"

int
socket_broadcast(int s) {
  int opt = 1;
  return winsock2errno(setsockopt(s, SOL_SOCKET, SO_BROADCAST, &opt, sizeof opt));
}
