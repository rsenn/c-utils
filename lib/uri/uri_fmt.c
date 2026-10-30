#include "../uri.h"
#include "../fmt.h"
#include "../str.h"

/* uri_fmt omits the port when it is the default of http or https */
size_t
uri_fmt(char* x, const uri_t* u) {
  size_t r = 0;

  if(u->proto) {
    r += fmt_str(x ? &x[r] : 0, u->proto);
    r += fmt_str(x ? &x[r] : 0, "://");
  }

  if(u->username) {
    r += fmt_str(x ? &x[r] : 0, u->username);

    if(u->password) {
      r += fmt_str(x ? &x[r] : 0, ":");
      r += fmt_str(x ? &x[r] : 0, u->password);
    }
    r += fmt_str(x ? &x[r] : 0, "@");
  }

  if(u->host)
    r += fmt_str(x ? &x[r] : 0, u->host);

  if(u->port)
    if(!u->proto || !((str_equal(u->proto, "http") && u->port == 80) || (str_equal(u->proto, "https") && u->port == 443))) {
      r += fmt_str(x ? &x[r] : 0, ":");
      r += fmt_ulong(x ? &x[r] : 0, u->port);
    }

  if(u->location)
    r += fmt_str(x ? &x[r] : 0, u->location);

  if(u->anchor)
    r += fmt_str(x ? &x[r] : 0, u->anchor);

  return r;
}
