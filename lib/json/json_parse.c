#include "../byte.h"
#include "../charbuf.h"
#include "../json.h"
#include "../scan.h"
#include "../stralloc.h"

#include <ctype.h>

int
json_parse_getsa(charbuf* b, stralloc* sa) {
  charbuf_skip_pred(b, &isspace);
  if(charbuf_peek(b) != '"') return 0;
  charbuf_skip(b);
  for(;;) {
    char ch;
    if(charbuf_peekc(b, &ch) < 0) return 0;
    if(ch == '\\') {
      if(charbuf_nextc(b, &ch) < 0) return 0;
    } else if(ch == '"') {
      charbuf_skip(b);
      break;
    }
    stralloc_append(sa, &ch);
    charbuf_skip(b);
  }
  return 1;
}

int
json_parse_num(jsonval* j, charbuf* b) {
  char c = charbuf_peek(b);
  if(isdigit(c) || c == '-' || c == '.' || c == '+') {
    size_t n;
    stralloc num;
    stralloc_init(&num);
    j->type = JSON_INT;
    for(;;) {
      stralloc_append(&num, &c);
      if(charbuf_nextc(b, &c) < 0) return 0;

      if(!isdigit(c) && c != '.' && c != 'E' && c != 'e') {
        if(!(j->type == JSON_DOUBLE && c == '-')) break;
      }
      if(c == '.' || c == 'E' || c == 'e') j->type = JSON_DOUBLE;
    }
    stralloc_nul(&num);
    n = j->type == JSON_INT ? scan_longlong(num.s, &j->intv) : scan_double(num.s, &j->doublev);
    if(n > 0) return 1;
  }
  return 0;
}

int
json_parse_bool(jsonval* j, charbuf* b) {
  char c = charbuf_peek(b);
  if(c == 't' || c == 'f') {
    char v = (c == 't');
    const char* n = v ? "rue" : "alse";
    charbuf_skip(b);
    for(; *n; ++n) {
      if(*n != charbuf_peek(b)) return 0;
      charbuf_skip(b);
    }
    j->type = JSON_BOOL;
    j->boolv = v;
    return 1;
  }
  return 0;
}

int
json_parse_array(jsonval* j, charbuf* b) {
  if(charbuf_peek(b) == '[') {
    slink** ptr = &j->listv;
    j->type = JSON_ARRAY;

    charbuf_skip(b);

    for(;; ptr = &(*ptr)->next) {
      if((*ptr = malloc(sizeof(jsonval) + sizeof(slink))) == NULL) return 0;
      byte_zero(*ptr, sizeof(jsonval) + sizeof(slink));

      if(!json_parse((jsonval*)&(*ptr)[1], b)) { break; }

      charbuf_skip_pred(b, &isspace);
      if(charbuf_peek(b) != ',') break;
      charbuf_skip(b);
    }
    if(charbuf_peek(b) != ']') return 0;
    charbuf_skip(b);

    return 1;
  }
  return 0;
}

int
json_parse_object(jsonval* j, charbuf* b) {
  if(charbuf_peek(b) == '{') {
    stralloc key;
    stralloc_init(&key);
    j->type = JSON_OBJECT;
    charbuf_skip(b);
    hmap_init(1024, &j->dictv);
    for(;;) {
          jsonval* member;
      stralloc_zero(&key);
      if(!json_parse_getsa(b, &key)) return 0;
      charbuf_skip_pred(b, &isspace);
      if(charbuf_peek(b) != ':') return 0;
      charbuf_skip(b);
     member = json_newnode(JSON_NONE);
      hmap_add(&j->dictv, key.s, key.len, 1, HMAP_DATA_TYPE_CUSTOM, member);
      if(!json_parse(member, b)) return 0;
      charbuf_skip_pred(b, &isspace);
      if(charbuf_peek(b) != ',') break;
      charbuf_skip(b);
    }
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
  if((r = json_parse_object(j, b))) return r;
  if((r = json_parse_array(j, b))) return r;
  if((r = json_parse_bool(j, b))) return r;
  if((r = json_parse_num(j, b))) return r;
  if((r = json_parse_string(j, b))) return r;
  return r;
}
