#ifndef HTTP_INTERNAL_H
#define HTTP_INTERNAL_H

#include "http.h"
#include "uint64.h"

enum { CHUNKED_SIZE = 0, CHUNKED_DATA, CHUNKED_END };

typedef struct {
  int state;
  uint64 remaining;
  buffer* other;
  char buf[1024];
} chunked;

ssize_t chunked_line(buffer* in, char* x, size_t n);

#endif /* defined HTTP_INTERNAL_H */
