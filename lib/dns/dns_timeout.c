#include "../dns.h"

/* dns_timeout_ms replaces the built-in timeouts of the transmit code when it
 * is not 0, for every attempt. */
unsigned long dns_timeout_ms = 0;

/* dns_timeout sets t to the time an attempt may take, secs by default. */
void
dns_timeout(struct taia* t, unsigned int secs) {
  if(!dns_timeout_ms) {
    taia_uint(t, secs);
    return;
  }
  taia_uint(t, (unsigned int)(dns_timeout_ms / 1000));
  t->nano = (dns_timeout_ms % 1000) * 1000000;
}
