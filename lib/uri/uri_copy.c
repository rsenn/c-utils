#include "../uri.h"
#include "../str.h"

void
uri_copy(uri_t* to, const uri_t* from) {
  to->proto = from->proto ? str_dup(from->proto) : 0;
  to->host = from->host ? str_dup(from->host) : 0;
  to->username = from->username ? str_dup(from->username) : 0;
  to->password = from->password ? str_dup(from->password) : 0;
  to->location = from->location ? str_dup(from->location) : 0;
  to->anchor = from->anchor ? str_dup(from->anchor) : 0;
  to->port = from->port;
}
