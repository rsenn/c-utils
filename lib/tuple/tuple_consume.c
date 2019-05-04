#include "../tuple.h"

tuple
tuple_consume(tuple t) {
  tuple_fn* fn = t.left.p;
  return fn(*t.right.t);
}
