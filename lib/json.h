/**
 * @defgroup   json
 * @brief      JSON module.
 * @{
 */
#ifndef JSON_H
#define JSON_H

#include "map.h"

#include "charbuf.h"
#include "slist.h"
#include "uint64.h"
#include "str.h"
#include "stralloc.h"
#include <sys/types.h>
#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  JSON_UNDEFINED = -1,
  JSON_NULL = 0,
  JSON_BOOL,
  JSON_INT,
  JSON_DOUBLE,
  JSON_STRING,
  JSON_ARRAY,
  JSON_OBJECT
} jsondata;

typedef enum key_type { JSON_KEY_OBJECT = 0, JSON_KEY_ARRAY = 1 } jsonkey_type;

typedef struct {
  jsonkey_type is_array;
  union {
    int64 intk;
    char* stringk;
  };
} jsonkey;

union jsonitem;

typedef struct {
  jsondata type;
  union {
    int boolv : 1;
    int64 intv;
    double doublev;
    stralloc stringv;
    slink* listv;
    union jsonitem* itemv;
    MAP_T dictv;
  };
} jsonval;

typedef union jsonitem {
  slink link;
  struct {
    union jsonitem* next;
    jsonval value;
  };
} jsonitem;

typedef struct {
  charbuf* b;
  jsonval* doc;
  jsonval** loc;
} jsonreader;

typedef union {
  const char* ws[5];
  struct {
    const char *indent, *newline, *spacing, *separat;
    char quote[2];
    int precision, depth, index;
    int compliant : 1;
  };
} jsonfmt;

enum {
  JSON_FMT_INDENT = 0,
  JSON_FMT_NEWLINE = 1,
  JSON_FMT_SPACING = 2,
  JSON_FMT_SEPARATOR = 3,
  JSON_FMT_QUOTE = 4,
};

typedef int json_read_callback_fn(jsonreader* r, jsondata id, stralloc* name, stralloc* value, MAP_T* attrs);
typedef void json_print_fn(jsonfmt*, jsonval*, int, int, char);
typedef void json_format_fn(jsonfmt*, jsonval*, int, int, char);
typedef int json_predicate_fn();

jsonitem* json_array_items(jsonval);
jsonitem* json_array_at(jsonval, int64);

jsonitem* json_append(jsonitem**, const jsonval);
void json_free(jsonval*);
jsonval json_property(jsonval, jsonval);
int json_isnull(jsonval);
int64 json_length(jsonval);
jsonval* json_newnode(jsondata);
jsonval json_object(void);
int json_parse_getsa(charbuf*, stralloc*, _Bool);
int json_parse_num(jsonval*, charbuf*);
int json_parse_bool(jsonval*, charbuf*);
int json_parse_null_or_undefined(jsonval*, charbuf*);
int json_parse_array(jsonval*, charbuf*);
int json_parse_object(jsonval*, charbuf*);
int json_parse_string(jsonval*, charbuf*);
int json_parse(jsonval*, charbuf*);
jsonval* json_array_push(jsonval*, const jsonval);
void json_read_callback(jsonreader*, json_read_callback_fn*);
jsonval* json_read_tree(charbuf*);
void json_reader_init(jsonreader*, charbuf*);
void json_recurse(jsonval*, void (*)(), void*);
jsonval* json_set_property(jsonval*, jsonval, jsonval);
double json_todouble(jsonval);
int64 json_toint(jsonval);
const char* json_tostring(jsonval, stralloc*);
void json_compact_printer(jsonfmt*, jsonval*, int depth, int index, char quote);
void json_default_printer(jsonfmt*, jsonval*, int depth, int index, char quote);

#ifdef BUFFER_H
void json_print(jsonval, buffer* b, json_print_fn* p);
#endif
#ifdef STRALLOC_H
void json_tosa(jsonval, stralloc* sa, json_print_fn* p);
#endif

static inline int
json_is_identifier_char(int c) {
  return (isalnum(c) || c == '$' || c == '_' || c == '@');
}

static inline stralloc*
json_string_sa(jsonval* val) {
  return val->type == JSON_STRING ? &val->stringv : 0;
}

static inline const char*
json_string_cstr(jsonval* val) {
  if(val->type == JSON_STRING) {
    stralloc_nul(&val->stringv);
    return val->stringv.s;
  }
  return 0;
}

static inline jsonval
json_undefined() {
  jsonval ret;
  ret.type = JSON_UNDEFINED;
  return ret;
}
static inline jsonval
json_null() {
  jsonval ret;
  ret.type = JSON_OBJECT;
  byte_zero(&ret.dictv, sizeof(MAP_T));
  return ret;
}

static inline jsonval
json_array() {
  jsonval ret;
  ret.type = JSON_ARRAY;
  ret.listv = 0;
  return ret;
}

static inline jsonval
json_double(double n) {
  jsonval ret;
  ret.type = JSON_DOUBLE;
  ret.doublev = n;
  return ret;
}
static inline jsonval
json_int(int64 i) {
  jsonval ret;
  ret.type = JSON_INT;
  ret.intv = i;
  return ret;
}
static inline jsonval
json_bool(int b) {
  jsonval ret;
  ret.type = JSON_BOOL;
  ret.boolv = !!b;
  return ret;
}
static inline jsonval
json_string(const char* s) {
  jsonval ret;
  ret.type = JSON_STRING;
  ret.stringv.a = 1 + (ret.stringv.len = str_len(s));
  ret.stringv.s = (char*)str_ndup(s, ret.stringv.len);
  return ret;
}
static inline jsonval
json_stringn(const char* s, size_t n) {
  jsonval ret;
  ret.type = JSON_STRING;
  ret.stringv.s = (char*)str_ndup(s, n);
  ret.stringv.len = n;
  ret.stringv.a = n + 1;
  return ret;
}

static inline int
json_isnumber(jsonval v) {
  return v.type == JSON_INT || v.type == JSON_DOUBLE;
}

#ifdef __cplusplus
}
#endif
#endif /* JSON_H */
/** @} */
