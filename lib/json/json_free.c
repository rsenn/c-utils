#include "../alloc.h"
#include "../json.h"

struct free_stats { size_t num, max_depth; };

static void
json_free_count(jsonval* val, struct free_stats* st, int depth) {
  ++st->num;
  if(st->max_depth  < depth)
    st->max_depth = depth;
}

static void
json_free_val(jsonval* val) {
  switch(val->type) {
    case JSON_STRING: {
        stralloc_free(&val->stringv);
        break;
    }
    case JSON_OBJECT: {
        TUPLE *t, *next;
        hmap_foreach(val->dictv, t) {
          json_free_val(t->vals.val_custom);
        }
        hmap_destroy(&val->dictv);
        break;
    }
    case JSON_ARRAY: {
      slink *link, *next;
      for(link = val->listv; link; link = next) {
        next = link->next;

        json_free_val(slist_data(link));
        alloc_free(link);
      }
      val->listv = 0;
      break;
	}
  }
}

void
json_free(jsonval* val) {
  static struct free_stats stats;
  json_recurse(val, json_free_count, &stats);
  json_free_val(val);
#ifdef DEBUG
  buffer_puts(buffer_2, "vals free'd: ");
  buffer_putulong(buffer_2, stats.num);
  buffer_putnlflush(buffer_2);
  buffer_puts(buffer_2, "max depth: ");
  buffer_putulong(buffer_2, stats.max_depth);
  buffer_putnlflush(buffer_2);
#endif
}
