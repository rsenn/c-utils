#include "../scan.h"

size_t
scan_lineskip(const char* s, size_t limit) {
  const char *t, *u = s + limit;

  for(t = s; t < u; ++t) {
    if(*t == '\n') {
      ++t;
      break;
    }
  }

  return (size_t)(t - s);
}

size_t
scan_lineskip_escaped(const char* s, size_t limit) {
  const char *t, *u;
  u = s + limit;

  for(t = s; t < u; ++t) {
    if(*t == '\\') {
      ++t;
      continue;
    }

    if(*t == '\n') {
      ++t;
      break;
    }
  }
  return (size_t)(t - s);
}
