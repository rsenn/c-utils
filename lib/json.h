#ifndef LIB_JSON_H_
#define LIB_JSON_H_

#include "buffer.h"
#include "hmap.h"
#include "slist.h"
#include "uint64.h"
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
    int64 intv;
    double doublev;
    char* stringv;
    slist* listv;
    HMAP_DB* dictv;
  };
} jsonval;

typedef struct {
  buffer* b;
  jsonval* doc;
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
void     json_reader_init(jsonreader*, buffer*);
void     json_read_callback(jsonreader*, json_read_callback_fn*);
jsonval* json_read_tree(buffer*);

#ifdef __cplusplus
}
#endif
#endif /* LIB_JSON_H_ */
void     json_free(jsonval*);
jsonval* json_newnode(jsondata);
void     json_reader_init(jsonreader*, buffer*);
void     json_read_callback(jsonreader*, json_read_callback_fn*);
jsonval* json_read_tree(buffer*);
