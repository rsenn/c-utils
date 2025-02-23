#ifndef _LIST_H
#define _LIST_H

#include "strlist.h"
//#include "strarray.h"
//#include "slist.h"

typedef strlist LIST_T;
typedef const char* LIST_VALUE_T;
typedef const char* LIST_ITER_T;

#define LIST_SIZE(list) strlist_count(&(list))
#define LIST_NEW(list) strlist_init(&(list), '\0')
#define LIST_GET(list, index) strlist_at(&(list), (index))
#define LIST_DESTROY(list) strlist_free(&(list))
#define LIST_FOREACH(list, iter) strlist_foreach_s(&(list), iter)
#define LIST_DELETE(list, index) strlist_remove_at(&(list), (index))
#define LIST_SEARCH(list, value) strlist_index_of(&(list), (value))

#define LIST_DATA(list, iter) ((char*)(iter))
#define LIST_DATA_LEN(list, iter) byte_chr((iter), strlist_end((list)) - (iter), (list)->sep)

#define LIST_SET(list, index, value) LIST_INSERT(list, (key), str_len(key) + 1, (value), str_len(value) + 1)
#define LIST_VISIT_ALL(list, fn, arg) \
  { \
    LIST_PAIR_T t; \
    LIST_FOREACH(list, t) \
    fn(LIST_KEY(t), str_len(LIST_KEY(t)), LIST_VALUE(t), 0, arg); \
  }

#define LIST_PUSH_BACK(list, item) strlist_push(&(list), (item))
#define LIST_PUSH_FRONT(list, item) strlist_unshift(&(list), (item))

#endif /* defined _LIST_H */
