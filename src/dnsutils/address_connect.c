#include "address.h"
#include "../../lib/errmsg.h"
#include "../../lib/socket.h"

#include <unistd.h>

/* address_connect connects a blocking TCP socket to host, an address or a
 * name, at port. Returns the socket, or -1 after it reported the reason. */
int
address_connect(const char* host, uint16 port) {
  address_t addr;
  stralloc name;
  int s;

  if(!address_scan(host, &addr)) {
    stralloc_init(&name);
    stralloc_copys(&name, host);

    if(address_lookup(&name, &addr, false) < 0) {
      errmsg_warn("cannot resolve ", host, 0);
      return -1;
    }
  }

  if((s = (addr.ip6 ? socket_tcp6b : socket_tcp4b)()) == -1) {
    errmsg_warnsys("socket", 0);
    return -1;
  }

  if((addr.ip6 ? socket_connect6(s, addr.ip, port, addr.scope_id) : socket_connect4(s, addr.ip, port)) == -1) {
    errmsg_warnsys("connect to ", host, 0);
    close(s);
    return -1;
  }
  return s;
}
