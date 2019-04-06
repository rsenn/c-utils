#include "../uint64.h"
#include "../buffer.h"
#include "../json.h"
#include "../slist.h"
#include "../stralloc.h"

static const jsonprinter json_default_printer = {
  "  ", "\n", " ", ",\n"
};

static void json_print_val(jsonval* val, buffer* b, int depth, const jsonprinter* p);

static int
byte_fullfils_predicate(const char* x, size_t len, int (*pred)(int)) {
  size_t i;

  for(i = 0; i < len; ++i) {
    if(!pred(x[i]))
      return 0;
  }
  return !!i;
}

static void
json_print_key(buffer* b, const char* key, size_t key_len) {
  int quote = !byte_fullfils_predicate(key, key_len, json_is_identifier_char);
  if(quote)
    buffer_put(b, "\"", 1);
  buffer_put(b, key, key_len);
  if(quote)
    buffer_put(b, "\"", 1);
}

static void
json_print_str(buffer* b, const char* x, size_t len) {
  char tmp[6];
  buffer_put(b, "\"", 1);

  while(len--) {
    if(*x == '"' || *x == '\\')
      buffer_put(b, tmp, fmt_escapecharjson(tmp, *x));
    else
      buffer_PUTC(b, *x);
    ++x;
  }

  buffer_put(b, "\"", 1);
}

static void
json_print_object(jsonval* val, buffer* b, int depth, const jsonprinter* p) {
  TUPLE* t;
  buffer_putm_internal(b, "{", p->newline, 0);
  hmap_foreach(val->dictv, t) {
    int last = hmap_next(val->dictv, t) == NULL;
    buffer_putns(b, p->indent, depth + 1);
    json_print_key(b, t->key, t->key_len);
    buffer_putm_internal(b, ":", p->spacing, 0);
    json_print_val(t->vals.val_custom, b, depth + 1, p);
    buffer_puts(b, last ? p->newline : p->separator);
  }
  buffer_putns(b, p->indent, depth);
  buffer_puts(b, "}");
}

static void
json_print_array(jsonval* val, buffer* b, int depth, const jsonprinter* p) {
  slink* ptr;
  buffer_putm_internal(b, "[", p->newline, 0);
  buffer_putns(b, p->indent, depth + 1);

  slink_foreach(val->listv, ptr) {
    json_print_val(slist_data(ptr), b, depth + 1, p);

    if(slist_next(ptr)) {
      buffer_puts(b, p->separator);
      buffer_putns(b, p->indent, depth + 1);
    }
  }

  buffer_puts(b, p->newline);
  buffer_putns(b, p->indent, depth);
  buffer_puts(b, "]");
}

static void
json_print_val(jsonval* val, buffer* b, int depth, const jsonprinter* p) {
  switch(val->type) {
    case JSON_NONE: break;
    case JSON_STRING: json_print_str(b, val->stringv.s, val->stringv.len); break;
    case JSON_DOUBLE: buffer_putdouble(b, val->doublev, 15); break;
    case JSON_BOOL: buffer_puts(b, val->boolv ? "true" : "false"); break;
    case JSON_INT: buffer_putlonglong(b, val->intv); break;
    case JSON_OBJECT: json_print_object(val, b, depth, p); break;
    case JSON_ARRAY: json_print_array(val, b, depth, p); break;
  }
}

void
json_print(jsonval* val, buffer* b, const jsonprinter* p) {
  if(p == NULL)
    p = &json_default_printer;
  json_print_val(val, b, 0, p);
  buffer_puts(b, p->newline);
  buffer_flush(b);
}