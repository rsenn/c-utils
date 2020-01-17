#include "../uint64.h"
#include "../buffer.h"
#include "../hmap.h"
#include "../json.h"
#include "../slist.h"
#include "../stralloc.h"
#include "../fmt.h"
#include <assert.h>

static void
depth_fn(const jsonval* v, int* arg, int depth) {
  if(*arg < depth)
    *arg = depth;
}

static int
get_depth(const jsonval* v) {
  int depth = -1;
  json_recurse((jsonval*)v, depth_fn, &depth);
  return depth;
}

static void
json_default_printer(jsonfmt* p, const jsonval* v, int depth, int index, char quote) {
  int pretty = v && get_depth(v) > 1;
  p->indent = pretty ? "  " : "";
  p->newline = pretty ? "\n" : "";
  p->spacing = pretty ? " " : "";
  p->separat = pretty ? ", " : ",";
  p->quote[0] = quote;
  p->quote[1] = '\0';

  p->precision = 10;
  p->depth = depth;
  p->index = index;
};

static void json_print_val(jsonval* val, buffer* b, int depth, void (*p)(jsonfmt*, jsonval*, int, int));

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
json_print_separator(jsonval* val, buffer* b, int what, const jsonfmt* printer) {
  const char* s;

  s = printer->ws[what];
  buffer_puts(b, s);

  if(s[str_chr(s, '\n')])
    buffer_putns(b, printer->indent, printer->depth);
}

static void
json_print_key(buffer* b, const char* key, size_t key_len, const jsonfmt* fmt) {
  char quote;
  quote = byte_fullfils_predicate(key, key_len, json_is_identifier_char) ? '\0' : fmt->quote[0];

  if(quote)
    buffer_putc(b, quote);
  buffer_put(b, key, key_len);
  if(quote)
    buffer_putc(b, quote);
}

static void
json_print_str(buffer* b, const char* x, size_t len, const jsonfmt* fmt) {
  char tmp[6];
  char quote = fmt->quote[0];
  if(quote)
    buffer_putc(b, quote);
  while(len--) {
    assert(*x);
    if(*x == quote || *x == '\\')
      buffer_put(b, tmp, fmt_escapecharjson(tmp, *x, quote));
    else
      buffer_PUTC(b, *x);
    ++x;
  }
  if(quote)
    buffer_putc(b, quote);
}

static void
json_print_object(jsonval* val, buffer* b, int depth, void (*p)(jsonfmt*, jsonval*, int, int)) {
  TUPLE* t;
  int index = 0;
  jsonfmt printer;
  p(&printer, val, depth, index);

  buffer_puts(b, "{");
  json_print_separator(val, b, JSON_FMT_NEWLINE, &printer);

  if(val->dictv && val->dictv->list_tuple) {

    hmap_foreach(val->dictv, t) {
      int last = hmap_next(val->dictv, t) == NULL;

      ++index;

      p(&printer, 0, depth + 1, index);
      json_print_key(b, t->key, t->key_len, &printer);

      buffer_puts(b, ":");
      json_print_separator(t->vals.val_custom, b, JSON_FMT_SPACING, &printer);

      json_print_val(t->vals.val_custom, b, depth, p);

      if(!last) {
        json_print_separator(t->vals.val_custom, b, JSON_FMT_SEPARATOR, &printer);
      }
    }
    p(&printer, val, depth - 1, -2);
    json_print_separator(val, b, JSON_FMT_NEWLINE, &printer);
  }
  buffer_puts(b, "}");
}

static void
json_print_array(jsonval* val, buffer* b, int depth, void (*p)(jsonfmt*, jsonval*, int, int)) {
  jsonfmt printer;
  slink* ptr;
  int index = 0;

  buffer_puts(b, "[");
  // buffer_puts(b, printer.spacing);
  //
  p(&printer, val, depth , index);
  json_print_separator(val, b, JSON_FMT_NEWLINE, &printer);

  slink_foreach(val->listv, ptr) {
    p(&printer, val, depth , index);
    json_print_val(slist_data(ptr), b, depth, p);
    ++index;
    if(slist_next(ptr)) {
      json_print_separator(val, b, JSON_FMT_SEPARATOR, &printer);
    }
  }
  p(&printer, val, depth - 1, -2);

  json_print_separator(val, b, JSON_FMT_NEWLINE, &printer);
  buffer_puts(b, "]");
}

static void
json_print_val(jsonval* val, buffer* b, int depth, void (*p)(jsonfmt*, jsonval*, int, int)) {
  jsonfmt printer;
  p(&printer, val, depth, 0);

  /*if(depth > 0)
    buffer_puts(b, printer.spacing);*/
  //    json_print_separator(val, b, depth, -1, p);

  switch(val->type) {
    case JSON_UNDEFINED: break;
    case JSON_STRING: json_print_str(b, val->stringv.s, val->stringv.len, &printer); break;
    case JSON_DOUBLE: buffer_putdouble(b, val->doublev, printer.precision); break;
    case JSON_BOOL: buffer_puts(b, val->boolv ? "true" : "false"); break;
    case JSON_INT: buffer_putlonglong(b, val->intv); break;
    case JSON_OBJECT: json_print_object(val, b, depth + 1, p); break;
    case JSON_ARRAY: json_print_array(val, b, depth + 1, p); break;
  }
}

void
json_print(jsonval val, buffer* b, void (*p)()) {
  jsonfmt printer;
  if(p == NULL)
    p = &json_default_printer;
  p(&printer, &val, 0, -1);
  json_print_val(&val, b, 0, p);
  buffer_puts(b, printer.newline);
  buffer_flush(b);
}
