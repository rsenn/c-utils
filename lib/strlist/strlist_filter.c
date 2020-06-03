#define _GNU_SOURCE
#include "../str.h"
#include "../strlist.h"

#include <string.h>
#include <stdlib.h>
/*
static int
sort_filter(const void* a, const void* b, void* arg) {
  strlist_filterfn_t* fn = arg;
  return fn(*(const char**)a, *(const char**)b);
}
*/
size_t
strlist_filter(strlist* sl, strlist_filterfn_t* filter_fn, strlist* out) {
  const char* x;
  size_t c = 0, n;

  strlist_foreach(sl, x, n) {
    if(filter_fn(x, n)) {
      strlist_pushb(out, x, n);
      c++;
    }
  }
  return c;
}
