#include "../tuple.h"
#include <stdlib.h>

variant
mktuple(variant left, variant right) {
  variant r;
  r.t = calloc(1, sizeof(tuple));
  r.t->left = left;
  r.t->right = right;
  return r;
}

