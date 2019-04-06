#ifndef LIB_JSON_H_
#define LIB_JSON_H_

#include "charbuf.h"
#include "hmap.h"
#include "slist.h"
#include "uint64.h"
#include <sys/types.h>
#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  JSON_NONE = 0,
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

void json_free(jsonval*);
jsonval* json_newnode(jsondata);
int json_parse_array(jsonval*, charbuf* b);
int json_parse_getsa(charbuf*, stralloc* sa);
int json_parse_object(jsonval*, charbuf* b);
int json_parse_string(jsonval*, charbuf* b);
int json_parse(jsonval*, charbuf* b);
void json_print(jsonval*, buffer* b, void (*)());
void json_reader_init(jsonreader*, charbuf* b);
void json_read_callback(jsonreader*, json_read_callback_fn* fn);
jsonval* json_read_tree(charbuf*);
void json_tosa(jsonval* val, stralloc* sa, void (*)());
void json_recurse(jsonval* val, void (*fn)(), void* arg);
int  json_set_property(jsonval* obj, jsonval name, jsonval value);
jsonval* json_get_property(jsonval* obj, jsonval name);
void json_tostring(jsonval* val, stralloc* sa);
double json_todouble(jsonval* val);
int64 json_toint(jsonval* val);

static inline int json_is_identifier_char(int c) {
  return isalpha(c) || c == '$' || c == '_' || ispunct(c);
}

static inline jsonval
json_double(double n) {jsonval ret = { JSON_DOUBLE }; ret.doublev = n; return ret; }

static inline jsonval
json_int(int64 i) { jsonval ret = { JSON_INT }; ret.intv = i; return ret; }

#ifdef STR_H
static inline jsonval
json_string(const char* s) { jsonval ret = { JSON_STRING }; ret.stringv.s = s; ret.stringv.len = str_len(s); ret.stringv.a = 0; return ret; }
#endif

static inline jsonval
json_bool(int b) { jsonval ret = { JSON_BOOL }; ret.boolv = !!b; return ret; }

#ifdef __cplusplus
}
#endif
#endif /* LIB_JSON_H_ */
