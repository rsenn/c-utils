#include "../tuple.h"

tuple
tuple_produce(variant left, variant right) {  
  left.t->right.t = mktuple(left, right);
  tuple t = { left.t->right, right };
  return t;
}
