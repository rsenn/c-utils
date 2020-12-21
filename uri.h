#ifndef _URI_H
#define _URI_H

#include "lib/uint16.h"

typedef struct {
  const char* proto;
  const char* host;
  uint16 port;
  const char* location;
  const char* username;
  const char* password;
  const char* anchor;
} uri_t;

void uri_dump(buffer* b, const uri_t* u);
size_t uri_fmt(char* x, const uri_t* u);
void uri_free(uri_t* uri);
int uri_init(uri_t* u);
size_t uri_scan(uri_t* u, const char* x, size_t len);
size_t uri_find(const char* x, size_t len);
char* uri_str(const uri_t* u);
void uri_copy(uri_t* to, const uri_t* from);
void uri_clear_anchor(uri_t* uri);

#endif /* defined _URI_H */
