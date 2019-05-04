

#ifndef TUPLE_H
#define TUPLE_H

struct tuple_s;

typedef union {
  void* p;
  long* l;
  struct tuple_s* t;
} variant;

typedef struct tuple_s {
  variant left, right;
} tuple;

typedef struct tuple_s tuple;

typedef tuple tuple_fn(variant,variant);

variant mktuple(variant left, variant right);

tuple tuple_left(variant, variant);

tuple  tuple_consume(variant, variant);
tuple  tuple_produce(variant, variant);

#endif
