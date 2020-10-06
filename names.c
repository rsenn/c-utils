#include "names.h"

#define _GNU_SOURCE
#include <stdlib.h>

#include "lib/dns.h"
#include "lib/buffer.h"
#include "lib/stralloc.h"
#include "lib/byte.h"

void
names_get(const char* x, size_t len, array* a) {
  size_t n, pos;
  const char* p;
  for(pos = 0; pos < len; pos = n) {
    n = dns_packet_skipname(x, len, pos);
    p = &x[pos];
    array_catb(a, &p, sizeof(char*));
  }
}

array
names_array(const char* x, size_t len) {
  array a;
  array_init(&a);
  names_get(x, len, &a);
  return a;
}

char*
names_vector(const char* x, size_t len) {
  size_t n;
  array a;
  a = names_array(x, len);
  n = array_length(&a, sizeof(char*));
  *(char**)array_allocate(&a, sizeof(char*), n) = NULL;
  return array_start(&a);
}

void
names_print(const array* arr) {
  char** s;
  stralloc sa;
  stralloc_init(&sa);
  buffer_puts(buffer_2, "Names:\n");

  array_foreach_t(arr, s) {
    stralloc_zero(&sa);
    dns_domain_todot_cat(&sa, *s);

    buffer_putsa(buffer_2, &sa);
    buffer_putnlflush(buffer_2);
  }
  stralloc_free(&sa);
}

int
names_compare(const void* aptr, const void* bptr) {
  const char* a = *(const char**)aptr;
  const char* b = *(const char**)bptr;
  return dns_domain_diff(a, b);
}

void
names_shuffle(char* x, size_t len) {
  array a;
  char** s;
  size_t n;
  array_init(&a);
  names_get(x, len, &a);
  array_sort(&a, sizeof(char*), &names_compare);
  array_foreach_t(&a, s) {
    n = dns_domain_length(*s);
    ++n;
    byte_copy(x, n, *s);
    x += n;
    len -= n;
  }
}