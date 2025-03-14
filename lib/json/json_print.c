#include "../json_internal.h"
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
buffer_indent_write(fd_type fd, char* x, size_t n, void* ptr) {
  buffer* b = (buffer*)ptr;
  struct indent_write* iw = (struct indent_write*)b->cookie;
  ssize_t ret;

  while(n > 0) {
    ret = iw->realop(fd, x, 1, ptr);

    if(*x == '\n') {
      size_t i;

      for(i = 0; i < iw->level; i++)
        if((ret = iw->realop(fd, (void*)iw->indent, str_len(iw->indent), ptr)) < 0)
          return ret;
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

static void json_print_val(jsonval* val, buffer* b, int depth, json_print_fn*);

static int
byte_fullfils_predicate(const char* x, size_t len, int (*pred)(int)) {
  size_t i;

  for(i = 0; i < len; ++i)
    if(!pred(x[i]))
      return 0;

  return !!i;
}

static void
json_print_separator(jsonval* val, buffer* b, int what, const jsonfmt* printer) {
  const char* s = printer->ws[what];

  buffer_puts(b, s);

  if(s[str_chr(s, '\n')])
    buffer_putns(b, printer->indent, printer->depth);
}

static void
json_print_key(buffer* b, const char* key, size_t key_len, const jsonfmt* fmt) {
  char quote = ((!isdigit(key[0]) && byte_fullfils_predicate(key, key_len, json_is_identifier_char))) ? fmt->quote[1] : fmt->quote[0];

  if(fmt->compliant)
    if(quote != '"')
      quote = '"';

  if(quote)
    buffer_putc(b, quote);

  buffer_put(b, key, key_len);

  if(quote)
    buffer_putc(b, quote);
}

static void
json_print_str(buffer* b, const char* x, size_t len, const jsonfmt* fmt) {
  char tmp[6], quote = fmt->quote[0];

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
  int index = 0;
  jsonfmt printer;

  p(&printer, val, depth + 1, index, 0);

  if(MAP_ISNULL(val->dictv)) {
    buffer_puts(b, "null");
    return;
  }

  buffer_puts(b, "{");
  p(&printer, val, depth + 1, index, 0);

  if(!MAP_ISNULL(val->dictv)) {
    MAP_PAIR_T pair;

    MAP_FOREACH(val->dictv, pair) {
      const char* key = MAP_KEY(pair);
      size_t klen = MAP_KEY_LEN(pair);
      jsonval* value = MAP_VALUE(pair);

      if(index == 0)
        json_print_separator(val, b, JSON_FMT_NEWLINE, &printer);
      else
        json_print_separator(value, b, JSON_FMT_SEPARATOR, &printer);

      ++index;
      p(&printer, 0, depth + 1, index, 0);
      json_print_key(b, key, klen, &printer);
      buffer_puts(b, ":");
      json_print_separator(value, b, JSON_FMT_SPACING, &printer);
      json_print_val(value, b, depth + 1, p);
    }

    p(&printer, val, depth + 1, -2, 0);
    json_print_separator(val, b, JSON_FMT_NEWLINE, &printer);
  }

  buffer_puts(b, "}");
}

static void
json_print_array(jsonval* val, buffer* b, int depth, json_print_fn* p) {
  jsonfmt printer;
  jsonitem* item;
  int index = 0;

  buffer_puts(b, "[");

  if(val->listv == NULL) {
    buffer_puts(b, "]");
    return;
  }

  p(&printer, val, depth + 1, index, 0);
  json_print_separator(val, b, JSON_FMT_NEWLINE, &printer);

  for(item = val->itemv; item; item = item->next) {
    p(&printer, val, depth + 1, index, 0);

    if(index > 0)
      json_print_separator(val, b, JSON_FMT_SEPARATOR, &printer);
    json_print_val(&item->value, b, depth + 1, p);
    ++index;
  }

  p(&printer, val, depth + 1, -2, 0);
  json_print_separator(val, b, JSON_FMT_NEWLINE, &printer);
  buffer_puts(b, "]");
}

static void
json_print_val(jsonval* val, buffer* b, int depth, json_print_fn* p) {
  jsonfmt printer;

  p(&printer, val, depth, 0, 0);

  switch(val->type) {
    case JSON_UNDEFINED: break;
    case JSON_NULL: buffer_puts(b, "null"); break;
    case JSON_STRING: json_print_str(b, val->stringv.s, val->stringv.len, &printer); break;
    case JSON_DOUBLE: buffer_putdouble(b, val->doublev, printer.precision); break;
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
