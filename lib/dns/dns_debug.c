#include "../buffer.h"
#include "../dns.h"

/* dns_verbose and dns_debug are set by the client programs, the library
 * writes to stderr when they are. dns_debug only has an effect when the
 * library was built with DEBUG_OUTPUT. */
int dns_verbose = 0;
int dns_debug = 0;

/* dns_debug_msg writes "dns: debug: a b c" if dns_debug is set, parts may
 * be NULL. */
void
dns_debug_msg(const char* a, const char* b, const char* c) {
  if(!dns_debug)
    return;
  buffer_puts(buffer_2, "dns: debug: ");
  if(a)
    buffer_puts(buffer_2, a);
  if(b)
    buffer_puts(buffer_2, b);
  if(c)
    buffer_puts(buffer_2, c);
  buffer_putnlflush(buffer_2);
}

/* dns_debug_num writes "dns: debug: a<n>b" if dns_debug is set. */
void
dns_debug_num(const char* a, unsigned long n, const char* b) {
  if(!dns_debug)
    return;
  buffer_puts(buffer_2, "dns: debug: ");
  if(a)
    buffer_puts(buffer_2, a);
  buffer_putulong(buffer_2, n);
  if(b)
    buffer_puts(buffer_2, b);
  buffer_putnlflush(buffer_2);
}

/* dns_debug_hex dumps buf as 16 bytes per line with offset and text. */
void
dns_debug_hex(const char* what, const char* buf, unsigned int len) {
  unsigned int i, j;
  static const char hex[] = "0123456789abcdef";
  char line[3];

  if(!dns_debug)
    return;
  buffer_puts(buffer_2, "dns: debug: hex dump of ");
  buffer_puts(buffer_2, what);
  buffer_puts(buffer_2, " packet, ");
  buffer_putulong(buffer_2, len);
  buffer_putsflush(buffer_2, " bytes:\n");

  for(i = 0; i < len; i += 16) {
    buffer_puts(buffer_2, "dns: debug:   ");
    buffer_putxlong0(buffer_2, i, 4);
    buffer_puts(buffer_2, "  ");
    for(j = i; j < i + 16; ++j) {
      if(j < len) {
        line[0] = hex[(unsigned char)buf[j] >> 4];
        line[1] = hex[buf[j] & 15];
        line[2] = ' ';
        buffer_put(buffer_2, line, 3);
      } else
        buffer_puts(buffer_2, "   ");
    }
    buffer_puts(buffer_2, " ");
    for(j = i; j < i + 16 && j < len; ++j)
      buffer_putc(buffer_2, buf[j] >= 32 && buf[j] < 127 ? buf[j] : '.');
    buffer_puts(buffer_2, "\n");
  }
  buffer_flush(buffer_2);
}
