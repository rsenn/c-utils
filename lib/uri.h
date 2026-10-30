#ifndef URI_H
#define URI_H

#include "uint16.h"
#include "buffer.h"

typedef struct {
  const char* proto;
  const char* host;
  uint16 port;
  const char* location;
  const char* username;
  const char* password;
  const char* anchor;
} uri_t;

/* writes a one-line description of u to b, without flushing */
void uri_dump(buffer* b, const uri_t* u);

/* formats u to x and returns the length, x == NULL only measures */
size_t uri_fmt(char* x, const uri_t* u);

/* frees all strings of u */
void uri_free(uri_t* uri);

/* zeroes u, returns 1 */
int uri_init(uri_t* u);

/* parses the uri in x[len] into u and returns the length consumed; fields
 * not present in x keep their previous value, so a relative uri resolves
 * against u */
size_t uri_scan(uri_t* u, const char* x, size_t len);

/* returns the index of the first "scheme://" in x[len], len if none */
size_t uri_find(const char* x, size_t len);

/* formats u into a new string, free with alloc_free */
char* uri_str(const uri_t* u);

/* copies u to a new set of strings */
void uri_copy(uri_t* to, const uri_t* from);

/* frees the anchor of uri */
void uri_clear_anchor(uri_t* uri);

#endif /* defined URI_H */
