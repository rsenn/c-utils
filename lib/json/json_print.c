#include "../uint64.h"
#include "../buffer.h"
#include "../hmap.h"
#include "../json.h"
#include "../slist.h"
#include "../stralloc.h"
#include "../fmt.h"

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
json_default_printer(jsonfmt* p, const jsonval* v, int depth, int index) {
  int pretty = get_depth(v) > 1;
  p->indent = pretty ? "  " : "";
  p->newline = pretty ? "\n" : "";
  p->spacing = pretty ? " " : "";
  p->quote = '"';
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
json_print_separator(jsonval* val, buffer* b, int what, int depth, int index, void (*p)(jsonfmt*, jsonval*, int, int)) {
  const char* s;
  jsonfmt printer;
  p(&printer, val, depth, index);

  s = printer.ws[what];
  buffer_puts(b, s);

  if(s[str_chr(s, '\n')])
    buffer_putns(b, printer.indent, depth);
}

static void
json_print_key(buffer* b, const char* key, size_t key_len, const jsonfmt* fmt) {
  int quote = !byte_fullfils_predicate(key, key_len, json_is_identifier_char);
  if(quote)
    buffer_puts(b, fmt->quote);
  buffer_put(b, key, key_len);
  if(quote)
    buffer_puts(b, fmt->quote);
}

static void
json_print_str(buffer* b, const char* x, size_t len, const jsonfmt* fmt) {
  char tmp[6];
  buffer_puts(b, fmt->quote);
  while(len--) {
    if(*x == fmt->quote[0] || *x == '\\')
      buffer_put(b, tmp, fmt_escapecharjson(tmp, *x, fmt->quote[0]));
    else
      buffer_PUTC(b, *x);
    ++x;
  }
  buffer_puts(b, fmt->quote);
}

static void
json_print_object(jsonval* val, buffer* b, int depth, void (*p)(jsonfmt*, jsonval*, int, int)) {
  TUPLE* t;
  int index = 0;
  jsonfmt printer;
  p(&printer, val, depth, index);

  buffer_puts(b, "{");
  // buffer_putm_internal(b, "{", printer.spacing, 0);
  json_print_separator(val, b, JSON_FMT_NEWLINE, depth, index, p);

  if(val->dictv && val->dictv->list_tuple) {
    hmap_foreach(val->dictv, t) {
      int last = hmap_next(val->dictv, t) == NULL;

      ++index;

      json_print_key(b, t->key, t->key_len - 1, p);

      buffer_puts(b, ":");
      json_print_separator(val, b, JSON_FMT_SPACING, depth, index, p);

      json_print_val(t->vals.val_chars, b, depth, p);

      if(!last) {
        json_print_separator(val, b, JSON_FMT_SEPARATOR, depth, index, p);
      }
    }
    json_print_separator(val, b, JSON_FMT_NEWLINE, depth - 1, -2, p);
  }
  buffer_puts(b, "}");
}

static void
json_print_array(jsonval* val, buffer* b, int depth, void (*p)(jsonfmt*, jsonval*, int, int)) {
  jsonfmt printer;
  p(&printer, val, depth, 0);
  slink* ptr;
  int index = 0;

  buffer_puts(b, "[");
  // buffer_puts(b, printer.spacing);
  json_print_separator(val, b, JSON_FMT_NEWLINE, depth, index, p);

  slink_foreach(val->listv, ptr) {
    json_print_val(slist_data(ptr), b, depth, p);
    ++index;
    if(slist_next(ptr)) {
      json_print_separator(val, b, JSON_FMT_SEPARATOR, depth, index, p);
    }
  }

  json_print_separator(val, b, JSON_FMT_NEWLINE, depth - 1, -2, p);
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
    case JSON_DOUBLE: buffer_putdouble(b, val->doublev, 15); break;
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
