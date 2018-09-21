#include "../windoze.h"

#include <sys/types.h>

#if !WINDOWS_NATIVE
#include <netinet/in.h>
#include <sys/socket.h>
#endif
#include "../socket.h"

int
socket_mcloop4(int s, char loop) {
  return winsock2errno(setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof loop));
}
