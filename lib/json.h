#ifndef LIB_JSON_H_
#define LIB_JSON_H_

#include "charbuf.h"
#include "hmap.h"
#include "slist.h"
#include <sys/types.h>

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
    char boolv : 1;
    long long intv;
    double doublev;
    stralloc stringv;
    slist listv;
    HMAP_DB* dictv;
  };
} jsonval;

typedef struct {
  charbuf* b;
  jsonval* doc;
  jsonval** loc;
} jsonreader;

#define node_is_closing(n) ((n)->name[0] == '/')

typedef int(json_read_callback_fn)(jsonreader* r,
                                   jsondata id,
                                   stralloc* name,
                                   stralloc* value,
                                   HMAP_DB** attrs);

typedef int(json_predicate_fn)();

void     json_free(jsonval*);
jsonval* json_newnode(jsondata);
int      json_parse_array(jsonval*, charbuf* b);
int      json_parse_getsa(charbuf*, stralloc* sa);
int      json_parse_object(jsonval*, charbuf* b);
int      json_parse_string(jsonval*, charbuf* b);
int      json_parse(jsonval*, charbuf* b);
void     json_print(jsonval*, buffer* b);
void     json_reader_init(jsonreader*, charbuf* b);
void     json_read_callback(jsonreader*, json_read_callback_fn* fn);
jsonval* json_read_tree(charbuf*);

#ifdef __cplusplus
}
#endif
#endif /* LIB_JSON_H_ */
