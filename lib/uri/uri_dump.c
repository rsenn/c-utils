#include "../uri.h"

void
uri_dump(buffer* b, const uri_t* u) {
  buffer_puts(b, "uri { proto = ");
  buffer_putstr(b, u->proto ? u->proto : "NULL");

  if(u->username) {
    buffer_puts(b, ", username = ");
    buffer_putstr(b, u->username);
  }

  if(u->password) {
    buffer_puts(b, ", password = ");
    buffer_putstr(b, u->password);
  }

  buffer_puts(b, ", host = ");
  buffer_putstr(b, u->host ? u->host : "NULL");

  if(u->port) {
    buffer_puts(b, ", port = ");
    buffer_putulong(b, u->port);
  }

  buffer_puts(b, ", location = ");
  buffer_putstr(b, u->location ? u->location : "NULL");

  if(u->anchor) {
    buffer_puts(b, ", anchor = ");
    buffer_putstr(b, u->anchor);
  }
  buffer_puts(b, " }");
}
