#include "../uint64.h"
#include "../buffer.h"
#include "../json.h"
#include "../slist.h"
#include "../stralloc.h"


static void
depth_fn(jsonval* v, int* arg, int depth) {
  if(*arg < depth)
    *arg = depth;
}

static int
get_depth(jsonval* v) {
  int depth = -1;
  json_recurse(v, depth_fn, &depth);
  return depth;
}


static void
json_default_printer(jsonfmt* p, const jsonval* v, int depth) {
  int pretty = get_depth(v) > 1;
  p->indent = pretty ? "  " : "";
  p->newline = pretty ? "\n" : "";
  p->spacing = pretty ? " " : "";
};

static void json_print_val(jsonval* val, buffer* b, int depth, void (*p)(jsonfmt*,jsonval*,int));

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
json_print_separator(jsonval* val, buffer* b, int depth, void (*p)(jsonfmt*,jsonval*,int)) {
  jsonfmt printer;
  p(&printer, val, depth);

  buffer_puts(b, printer.newline);

  if(printer.newline[str_chr(printer.newline, '\n')])
    buffer_putns(b, printer.indent, depth);
  else
    buffer_puts(b, printer.spacing);
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
json_print_object(jsonval* val, buffer* b, int depth, void (*p)(jsonfmt*,jsonval*,int)) {
  TUPLE* t;
  jsonfmt printer;
  p(&printer, val, depth);

  buffer_puts(b, "{");
    json_print_separator(val, b, depth + 1, p);

  hmap_foreach(val->dictv, t) {
    int last = hmap_next(val->dictv, t) == NULL;

    json_print_key(b, t->key, t->key_len);

    buffer_putm_internal(b, ":", printer.spacing, 0);

    json_print_val(t->vals.val_custom, b, depth + 1, p);

    if(!last) {
      buffer_put(b, ",", 1);
      json_print_separator(val, b, depth + 1, p);
    }
  }
  json_print_separator(val, b, depth, p);
  buffer_puts(b, "}");
}


static void
json_print_array(jsonval* val, buffer* b, int depth, void (*p)(jsonfmt*,jsonval*,int)) {
  slink* ptr;

  buffer_puts(b, "[");
  json_print_separator(val, b, depth + 1, p);

  slink_foreach(val->listv, ptr) {
    json_print_val(slist_data(ptr), b, depth + 1, p);

    if(slist_next(ptr)) {
      buffer_put(b, ",", 1);
      json_print_separator(val, b, depth + 1, p);
    }
  }

  json_print_separator(val, b, depth, p);
  buffer_puts(b, "]");
}

static void
json_print_val(jsonval* val, buffer* b, int depth, void (*p)(jsonfmt*,jsonval*,int)) {
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
json_print(jsonval* val, buffer* b, void (*p)(jsonfmt*,jsonval*,int)) {
  jsonfmt printer;
  if(p == NULL)
    p = &json_default_printer;
  p(&printer, val, 0);
  json_print_val(val, b, 0, p);
  buffer_puts(b, printer.newline);
  buffer_flush(b);
}