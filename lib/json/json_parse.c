#include "../uint64.h"
#include "../buffer.h"
#include "../json_internal.h"
#include "../bool.h"
#include "../byte.h"
#include "../charbuf.h"
#include "../scan.h"
#include "../stralloc.h"

#include <ctype.h>

int
json_parse_getsa(charbuf* b, stralloc* out, bool quoted) {
  unsigned char ch;
  int ret;
  size_t i;
  stralloc sa;
  stralloc_init(&sa);

  for(; (ret = charbuf_peekc(b, &ch)) > 0; charbuf_skip(b)) {
    if(ch == '\\') {
      if((ret = charbuf_nextc(b, &ch)) <= 0)
        return ret;
    } else if(quoted && ch == '"') {
      charbuf_skip(b);
      ret = 1;
      break;
    } else if(!quoted && ch == ':') {
      ret = sa.len > 0;
      break;
    }
    stralloc_APPEND(&sa, &ch);
  }
  if(ret > 0 && out) {
    for(i = 0; i < sa.len; i++) {
      size_t r;
      unsigned int n;
      if((r = scan_utf8(&sa.s[i], sa.len - i, &n)) > 1) {
        stralloc_cats(out, "\\u");
        stralloc_catxlong0(out, n, n > 0xffffffu ? 8 : n > 0xffffu ? 6 : 4);
        i += r - 1;
      } else {
        ch = n;
        stralloc_APPEND(out, &ch);
      }
    }
  }

  stralloc_free(&sa);
  return ret;
}

int
json_parse_num(jsonval* j, charbuf* b) {
  size_t n;
  stralloc num;
  int ret;
  unsigned char c;
  if((ret = charbuf_peek(b)) <= 0)
    return ret;
  c = ret;
  if(isdigit(c) || c == '-' || c == '.' || c == '+') {
    j->type = JSON_INT;
    stralloc_init(&num);
    for(;;) {
      stralloc_APPEND(&num, &c);
      if((ret = charbuf_nextc(b, &c)) <= 0)
        return ret;
      if(!isdigit(c) && c != '.' && c != 'E' && c != 'e') {
        if(!(j->type == JSON_DOUBLE && c == '-'))
          break;
      }
      if(c == '.' || c == 'E' || c == 'e')
        j->type = JSON_DOUBLE;
    }
    stralloc_nul(&num);
    n = j->type == JSON_INT ? scan_longlong(num.s, &j->intv)
                            : scan_double(num.s, &j->doublev);
    stralloc_free(&num);
    if(n > 0) {
      return 1;
    }
  }
  return 0;
}

int
json_parse_bool(jsonval* j, charbuf* b) {
  int ret;
  if((ret = charbuf_skip_ifset(b, "tf", 2)) > 0) {
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
  return ret;
}

int
json_parse_null_or_undefined(jsonval* j, charbuf* b) {
  int ret;
  if((ret = charbuf_skip_ifset(b, "nu", 2)) > 0) {
    const char* n;
    int v = charbuf_peek(b) == 'u';
    for(n = v ? "ull" : "ndefined"; *n; ++n) {
      if(!charbuf_skip_ifeq(b, *n))
        return 0;
    }
    j->type = v ? JSON_OBJECT : JSON_UNDEFINED;
    MAP_ZERO(j->dictv);
    return 1;
  }
  return ret;
}

int
json_parse_array(jsonval* j, charbuf* b) {
  if(charbuf_skip_ifeq(b, '[')) {
    int ret;
    jsonitem **ptr, *item;
    unsigned char c;
    int64_t i = 0;

    j->type = JSON_ARRAY;
    j->listv = 0;

    ptr = &j->itemv;

    charbuf_skip_pred(b, isspace);

    for(; (ret = charbuf_peekc(b, &c)) > 0;) {

      if(c == ']') {
        ret = 1;
        break;
      }
      if((item = json_append(ptr, json_undefined())) == 0)
        return -1;

      ptr = &item->next;

      // = slink_insert(ptr, lnk);

      if((ret = json_parse(&item->value, b)) <= 0)
        break;

      buffer_puts(buffer_2, "json array element ");
      buffer_putlonglong(buffer_2, i++);
      buffer_puts(buffer_2, ": ");
      json_print(item->value, buffer_2, json_compact_printer);
      buffer_putnlflush(buffer_2);

      charbuf_skip_pred(b, &isspace);

      if((ret = charbuf_getc(b, &c)) <= 0)
        return ret;

      if(c == ',') {
        charbuf_skip_pred(b, isspace);

        continue;
      }

      break;
    }

    if(c == ']')
      ret = 1;

    /*if(ret == 1)
      charbuf_skip_ifeq(b, ']');*/
    return ret;
  }
  return 0;
}

int
json_parse_object(jsonval* j, charbuf* b) {
  if(charbuf_skip_ifeq(b, '{')) {
    stralloc key;
    int ret;
    unsigned char c;
    j->type = JSON_OBJECT;
    MAP_NEW(j->dictv);
    stralloc_init(&key);
    charbuf_skip_pred(b, isspace);

    for(; (ret = charbuf_peekc(b, &c)) > 0;) {
      jsonval member = {.type = JSON_UNDEFINED};
      stralloc_zero(&key);
      if(c == '}') {

        charbuf_skip(b);
        return 1;
      }

      if((ret = json_parse_getsa(b, &key, charbuf_skip_ifeq(b, '"'))) <= 0)
        return ret;

      charbuf_skip_pred(b, &isspace);

      if(!charbuf_skip_ifeq(b, ':'))
        return 0;

      //      stralloc_nul(&key);

      charbuf_skip_pred(b, &isspace);
      if(!json_parse(&member, b))
        return 0;

      MAP_INSERT(j->dictv, key.s, key.len, &member, sizeof(member));

      charbuf_skip_pred(b, &isspace);
      if(charbuf_skip_ifeq(b, ',')) {
        charbuf_skip_pred(b, &isspace);
        continue;
      }
      break;
    }
    stralloc_free(&key);
    return ret;
  }
  return 0;
}

int
json_parse_string(jsonval* j, charbuf* b) {
  if(charbuf_skip_ifeq(b, '"')) {

    j->type = JSON_STRING;
    json_parse_getsa(b, &j->stringv, true);
    return 1;
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
