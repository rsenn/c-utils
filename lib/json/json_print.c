#include "../uint64.h"
#include "../buffer.h"
#include "../hmap.h"
#include "../json.h"
#include "../slist.h"
#include "../stralloc.h"
#include "../fmt.h"
#include <assert.h>

struct indent_write {
  buffer_op_proto* realop;
  size_t level;
  const char* indent;
};

static void
buffer_indent_set(buffer* b, const char* in) {
  struct indent_write* iw = (struct indent_write*)b->cookie;
  iw->indent = in;
}

static ssize_t
buffer_indent_write(fd_t fd, char* x, size_t n, void* ptr) {
  buffer* b = (buffer*)ptr;
  struct indent_write* iw = (struct indent_write*)b->cookie;

  ssize_t ret;
  while(n > 0) {

    ret = iw->realop(fd, x, 1, ptr);

    if(*x == '\n') {
      size_t i;
      for(i = 0; i < iw->level; i++) {
        ret = iw->realop(fd, (void*)iw->indent, str_len(iw->indent), ptr);
        if(ret < 0)
          return ret;
      }
    }
    x++;
    n--;
  }
  return ret;
}

static void
buffer_indent(buffer* b, struct indent_write* iw) {
  iw->realop = b->op;
  iw->indent = "  ";
  iw->level = 0;
  b->op = (buffer_op_proto*)&buffer_indent_write;
  b->cookie = iw;
}

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
json_default_printer(jsonfmt* p, jsonval* v, int depth, int index, char quote) {
  int pretty = v && get_depth(v) > 1;
  static char q[2] = {'"', '\0'};
  p->indent = pretty ? " " : "";
  p->newline = pretty ? "\n" : "";
  p->spacing = pretty ? " " : "";
  p->separat = pretty ? ", " : ",";
  p->quote[0] = quote;
  p->quote[1] = quote;
  p->precision = 10;
  p->depth = depth;
  p->index = index;
  p->compliant = 1;
};

static void json_print_val(jsonval* val, buffer* b, int depth, json_print_fn*);

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
json_print_separator(jsonval* val,
                     buffer* b,
                     int what,
                     const jsonfmt* printer) {
  const char* s;

  s = printer->ws[what];
  buffer_puts(b, s);

  if(s[str_chr(s, '\n')])
    buffer_putns(b, printer->indent, printer->depth);
}

static void
json_print_key(buffer* b, const char* key, size_t key_len, const jsonfmt* fmt) {
  char quote;
  quote = ((!isdigit(key[0]) &&
            byte_fullfils_predicate(key, key_len, json_is_identifier_char)))
              ? fmt->quote[1]
              : fmt->quote[0];
  if(fmt->compliant) {
    if(quote != '"')
      quote = '"';
  }
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
    if(*x == quote || (*x == '\\' && !(len > 0 && x[1] == 'u')))
      buffer_put(b, tmp, fmt_escapecharjson(tmp, *x, quote));
    else
      buffer_PUTC(b, *x);
    ++x;
  }
  if(quote)
    buffer_putc(b, quote);
}

static void
json_print_object(jsonval* val, buffer* b, int depth, json_print_fn* p) {
  TUPLE* t;
  int index = 0;
  jsonfmt printer;
  p(&printer, val, depth + 1, index, 0);
  if(val->dictv == NULL) {
    buffer_puts(b, "null");
    return;
  }
  buffer_puts(b, "{");
  p(&printer, val, depth + 1, index, 0);

  json_print_separator(val, b, JSON_FMT_NEWLINE, &printer);
  if(val->dictv && val->dictv->list_tuple) {
    hmap_foreach(val->dictv, t) {
      int last = hmap_next(val->dictv, t) == NULL;
      ++index;
      p(&printer, 0, depth + 1, index, 0);
      json_print_key(b, t->key, t->key_len, &printer);
      buffer_puts(b, ":");
      json_print_separator(t->vals.val_custom, b, JSON_FMT_SPACING, &printer);
      json_print_val(t->vals.val_custom, b, depth + 1, p);
      if(!last) {
        json_print_separator(t->vals.val_custom,
                             b,
                             JSON_FMT_SEPARATOR,
                             &printer);
      }
    }
    p(&printer, val, depth + 1, -2, 0);
    json_print_separator(val, b, JSON_FMT_NEWLINE, &printer);
  }
  buffer_puts(b, "}");
}

static void
json_print_array(jsonval* val, buffer* b, int depth, json_print_fn* p) {
  jsonfmt printer;
  union {
    slink* ptr;
    void* iter;
  } it;
  int index = 0;

  buffer_puts(b, "[");

  if(val->listv == NULL) {
    buffer_puts(b, "]");
    return;
  }
  // buffer_puts(b, printer.spacing);
  //
  p(&printer, val, depth + 1, index, 0);
  json_print_separator(val, b, JSON_FMT_NEWLINE, &printer);

  slink_foreach(val->listv, it.iter) {
    p(&printer, val, depth + 1, index, 0);
    if(index > 0)
      json_print_separator(val, b, JSON_FMT_SEPARATOR, &printer);

    json_print_val(slist_data(it.ptr), b, depth + 1, p);
    ++index;
    // if(!!slist_next(it.ptr)) json_print_separator(val, b, JSON_FMT_SEPARATOR,
    // &printer);
  }
  p(&printer, val, depth + 1, -2, 0);

  json_print_separator(val, b, JSON_FMT_NEWLINE, &printer);
  buffer_puts(b, "]");
}

static void
json_print_val(jsonval* val, buffer* b, int depth, json_print_fn* p) {
  jsonfmt printer;
  p(&printer, val, depth, 0, 0);

  /*if(depth > 0)
    buffer_puts(b, printer.spacing);*/
  //    json_print_separator(val, b, depth, -1, p);

  switch(val->type) {
    case JSON_UNDEFINED: break;
    case JSON_STRING:
      json_print_str(b, val->stringv.s, val->stringv.len, &printer);
      break;
    case JSON_DOUBLE:
      buffer_putdouble(b, val->doublev, printer.precision);
      break;
    case JSON_BOOL: buffer_puts(b, val->boolv ? "true" : "false"); break;
    case JSON_INT: buffer_putlonglong(b, val->intv); break;
    case JSON_OBJECT: json_print_object(val, b, depth, p); break;
    case JSON_ARRAY: json_print_array(val, b, depth, p); break;
  }
}

void
json_print(jsonval val, buffer* b, json_print_fn* p) {
  jsonfmt printer;
  if(p == NULL)
    p = &json_default_printer;
  p(&printer, &val, 0, -1, '"');
  json_print_val(&val, b, 0, p);
  buffer_puts(b, printer.newline);
  buffer_flush(b);
}
