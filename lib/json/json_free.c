#include "../json_internal.h"
#include "../alloc.h"
#include "../map.h"

struct free_stats {
  size_t num, max_depth;
};

static void
json_free_count(jsonval* val, struct free_stats* st, size_t depth) {
  ++st->num;
  if(st->max_depth < depth)
    st->max_depth = depth;
}

static void
json_free_val(jsonval* val, jsonval* parent) {
  switch(val->type) {
    case JSON_STRING: {
      stralloc_free(&val->stringv);
      break;
    }
    case JSON_OBJECT: {
      if(!MAP_ISNULL(val->dictv)) {
        MAP_PAIR_T t;
        MAP_FOREACH(val->dictv, t) { json_free_val(MAP_VALUE(t), val); }
        MAP_DESTROY(val->dictv);
      }
      break;
    }
    case JSON_ARRAY: {
      jsonitem *item, *next;
      for(item = val->itemv; item; item = next) {
        next = item->next;

        json_free_val(&item->value, val);
        slink_delete(item);
      }
      val->listv = 0;
      break;
    }
    default: break;
  }

  if(!parent || (parent->type != JSON_OBJECT && parent->type != JSON_ARRAY))
    alloc_free(val);
}

void
json_free(jsonval* val) {
  static struct free_stats stats;
  json_recurse(val, json_free_count, &stats);
  json_free_val(val, 0);
#ifdef JSON_DEBUG
  buffer_puts(buffer_2, "vals free'd: ");
  buffer_putulong(buffer_2, stats.num);
  buffer_putnlflush(buffer_2);
  buffer_puts(buffer_2, "max depth: ");
  buffer_putulong(buffer_2, stats.max_depth);
  buffer_putnlflush(buffer_2);
#endif
}
