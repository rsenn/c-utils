#ifndef LIB_JSON_H_
#define LIB_JSON_H_

#include "charbuf.h"
#include "hmap.h"
#include "slist.h"
#include "uint64.h"
#include "str.h"
#include <sys/types.h>
#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  JSON_UNDEFINED = 0,
  JSON_BOOL,
  JSON_INT,
  JSON_DOUBLE,
  JSON_STRING,
  JSON_ARRAY,
  JSON_OBJECT,
} jsondata;

typedef struct {
  jsondata type;
  union {
    int boolv : 1;
    int64 intv;
    double doublev;
    stralloc stringv;
    slink* listv;
    HMAP_DB* dictv;
  };
} jsonval;

typedef struct {
  charbuf* b;
  jsonval* doc;
  jsonval** loc;
} jsonreader;

typedef struct {
  const char* indent;
  const char* newline;
  const char* spacing;
  char quote;
} jsonfmt;

#define node_is_closing(n) ((n)->name[0] == '/')

typedef int(json_read_callback_fn)(jsonreader* r, jsondata id, stralloc* name, stralloc* value, HMAP_DB** attrs);

typedef int(json_predicate_fn)();

void        json_free(jsonval* val);
int64       json_length(jsonval v);
jsonval*    json_newnode(jsondata type);
int         json_parse_array(jsonval* j, charbuf* b);
int         json_parse_bool(jsonval* j, charbuf* b);
int         json_parse_getsa(charbuf* b, stralloc* sa);
int         json_parse(jsonval* j, charbuf* b);
int         json_parse_num(jsonval* j, charbuf* b);
int         json_parse_object(jsonval* j, charbuf* b);
int         json_parse_string(jsonval* j, charbuf* b);
void        json_print(jsonval val, buffer* b, void (*p)(void));
void        json_read_callback(jsonreader* r, json_read_callback_fn* fn);
void        json_reader_init(jsonreader* r, charbuf* b);
jsonval*    json_read_tree(charbuf* b);
void        json_recurse(jsonval* val, void (*fn)(void), void* arg);
int         json_set_property(jsonval* obj, jsonval name, jsonval value);
jsonval     json_get_property(jsonval obj, jsonval name);
double      json_todouble(jsonval val);
int64       json_toint(jsonval val);
void        json_tosa(jsonval val, stralloc* sa, void (*p)(jsonfmt* , jsonval* , int));
const char* json_tostring(jsonval val, stralloc* sa);


static inline int json_is_identifier_char(int c) {
  return isalpha(c) || c == '$' || c == '_' || ispunct(c);
}
static inline const char* json_str(jsonval* val) {
  if(val->type == JSON_STRING) {
    stralloc_nul(&val->stringv);
    return val->stringv.s;
  }
  return 0;
}

static inline jsonval json_undefined() { jsonval ret = { JSON_UNDEFINED }; return ret; }
static inline jsonval json_null() { jsonval ret = { JSON_OBJECT }; ret.dictv = 0; return ret; }
static inline jsonval json_object() { jsonval ret = { JSON_OBJECT }; ret.dictv = 0; hmap_init(MAP_BUCKET, &ret.dictv); return ret; }
static inline jsonval json_array() { jsonval ret = { JSON_ARRAY }; ret.listv = 0; return ret; }
static inline jsonval json_double(double n) {jsonval ret = { JSON_DOUBLE }; ret.doublev = n; return ret; }
static inline jsonval json_int(int64 i) { jsonval ret = { JSON_INT }; ret.intv = i; return ret; }
static inline jsonval json_bool(int b) { jsonval ret = { JSON_BOOL }; ret.boolv = !!b; return ret; }
static inline jsonval json_string(const char* s) { jsonval ret = { JSON_STRING }; ret.stringv.s = (char*)s; ret.stringv.len = str_len(s); ret.stringv.a = 0; return ret; }


static inline int json_isnull(jsonval v) { return v.type == JSON_OBJECT && v.dictv == 0; }
static inline int json_isnumber(jsonval v) { return v.type == JSON_INT || v.type == JSON_DOUBLE; }

#ifdef __cplusplus
}
#endif
#endif /* LIB_JSON_H_ */
