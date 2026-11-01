#include "../http.h"

const char*
http_method(http_req_t type) {
  switch(type) {
    case POST: return "POST";
    case PUT: return "PUT";
    case DELETE: return "DELETE";
    case PATCH: return "PATCH";
    case HEAD: return "HEAD";
    default: return "GET";
  }
}
