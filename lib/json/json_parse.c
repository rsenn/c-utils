#include "../hmap.h"
#include "../byte.h"
#include "../charbuf.h"
#include "../json.h"
#include "../scan.h"
#include "../stralloc.h"

#include <ctype.h>

int
json_parse_getsa(charbuf* b, stralloc* out) {
  char ch;
  int quoted = 0;
  size_t i;
  stralloc sa;
  stralloc_init(&sa);
  charbuf_skip_pred(b, isspace);
  quoted = charbuf_skip_ifeq(b, '"');

  for(;; charbuf_skip(b)) {
    if(charbuf_peekc(b, &ch) < 0)
      return 0;

    if(ch == '\\') {
      if(charbuf_nextc(b, &ch) < 0)
        return 0;

    } else if(quoted && ch == '"') {
      charbuf_skip(b);
      break;
    } else if(!quoted && ch == ':') {
      break;
    }

    stralloc_append(&sa, &ch);
  }
  for(i = 0; i < sa.len; i++) {
    size_t r;
    unsigned int n;
    if((r = scan_utf8(&sa.s[i], sa.len - i, &n)) > 1) {
      stralloc_cats(out, "\\u");
      stralloc_catxlong0(out, n, n > 0xffffffu ? 8 : n > 0xffffu ? 6 : 4);
      i += r - 1;
    } else {
      ch = n;
      stralloc_append(out, &ch);
    }
  }
  stralloc_free(&sa);
  return 1;
}

int
json_parse_num(jsonval* j, charbuf* b) {
  size_t n;
  stralloc num;
  char c = charbuf_peek(b);

  if(isdigit(c) || c == '-' || c == '.' || c == '+') {
    j->type = JSON_INT;
    stralloc_init(&num);

    for(;;) {
      stralloc_append(&num, &c);
      if(charbuf_nextc(b, &c) < 0)
        return 0;

      if(!isdigit(c) && c != '.' && c != 'E' && c != 'e') {
        if(!(j->type == JSON_DOUBLE && c == '-'))
          break;
      }
      if(c == '.' || c == 'E' || c == 'e')
        j->type = JSON_DOUBLE;
    }
    stralloc_nul(&num);
    n = j->type == JSON_INT ? scan_longlong(num.s, &j->intv) : scan_double(num.s, &j->doublev);
    stralloc_free(&num);
    if(n > 0) {
      return 1;
    }
  }
  return 0;
}

int
json_parse_bool(jsonval* j, charbuf* b) {
  if(charbuf_skip_ifset(b, "tf", 2)) {
    const char* n;
    int v = charbuf_peek(b) == 'r';
    for(n = v ? "rue" : "alse"; *n; ++n) {
      if(!charbuf_skip_ifeq(b, *n))
        return 0;
    }
    j->type = JSON_BOOL;
    j->boolv = v;
    return 1;
  }
  return 0;
}

int
json_parse_null_or_undefined(jsonval* j, charbuf* b) {
  if(charbuf_skip_ifset(b, "nu", 2)) {
    const char* n;
    int v = charbuf_peek(b) == 'u';
    for(n = v ? "ull" : "ndefined"; *n; ++n) {
      if(!charbuf_skip_ifeq(b, *n))
        return 0;
    }
    j->type = v ? JSON_OBJECT : JSON_UNDEFINED;
    j->dictv = NULL;
    return 1;
  }
  return 0;
}

int
json_parse_array(jsonval* j, charbuf* b) {
  slink** ptr;
  int i = 0;

  if(charbuf_skip_ifeq(b, '[')) {
    j->type = JSON_ARRAY;
    j->listv = 0;

    if(charbuf_skip_ifeq(b, ']'))
      return 1;

    ptr = &j->listv;

    for(;; ptr = &((*ptr)->next)) {
      if((*ptr = slink_new(jsonval)) == NULL)
        return 0;

      /* buffer_puts(buffer_2, "json array element ");
      buffer_putlong(buffer_2, i++);
      buffer_putnlflush(buffer_2); */

      if(!json_parse((jsonval*)&((*ptr)[1]), b))
        break;

      charbuf_skip_pred(b, &isspace);
      if(!charbuf_skip_ifeq(b, ','))
        break;
    }
    if(charbuf_skip_ifeq(b, ']'))
      return 1;
  }
  return 0;
}

int
json_parse_object(jsonval* j, charbuf* b) {
  stralloc key;
  if(charbuf_skip_ifeq(b, '{')) {
    stralloc_init(&key);
    j->type = JSON_OBJECT;
    hmap_init(1024, &j->dictv);

    for(;;) {
      jsonval* member;
      stralloc_zero(&key);
      if(!json_parse_getsa(b, &key))
        return 0;
      charbuf_skip_pred(b, &isspace);
      if(!charbuf_skip_ifeq(b, ':'))
        return 0;
      member = json_newnode(JSON_UNDEFINED);
      hmap_add(&j->dictv, key.s, key.len, 1, HMAP_DATA_TYPE_CUSTOM, member);
      if(!json_parse(member, b))
        return 0;
      charbuf_skip_pred(b, &isspace);
      if(!charbuf_skip_ifeq(b, ','))
        break;
    }

    stralloc_free(&key);
    if(charbuf_skip_ifeq(b, '}'))
      return 1;
  }
  return 0;
}

int
json_parse_string(jsonval* j, charbuf* b) {
  if(charbuf_peek(b) == '"') {
    j->type = JSON_STRING;
    return json_parse_getsa(b, &j->stringv);
  }
  return 0;
}

int
json_parse(jsonval* j, charbuf* b) {
  int r = 0;
  charbuf_skip_pred(b, &isspace);
  if(!(r = json_parse_object(j, b)))
    if(!(r = json_parse_array(j, b)))
      if(!(r = json_parse_bool(j, b)))
        if(!(r = json_parse_num(j, b)))
          if(!(r = json_parse_string(j, b)))
            r = json_parse_null_or_undefined(j, b);
  return r;
}
