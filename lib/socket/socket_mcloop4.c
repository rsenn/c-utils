#include <sys/types.h>
#if !defined(_WIN32) && !defined(_WIN64)
#include <netinet/in.h>
#include <sys/socket.h>
#endif
#include "../socket.h"
#include "../windoze.h"

int
socket_mcloop4(int s, char loop) {
  return winsock2errno(setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof loop));
}
