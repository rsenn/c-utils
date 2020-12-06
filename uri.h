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
} uri_t;

void uri_dump(buffer* b, const uri_t* u);
size_t uri_fmt(char* x, const uri_t* u);
char* uri_str(const uri_t* u);
void uri_free(uri_t* uri);
int uri_init(uri_t* u);
size_t uri_scan(const char* x, uri_t* u);

#endif /* defined _URI_H */
