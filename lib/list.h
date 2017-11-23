#ifndef LIST_H
#define LIST_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct list {
  struct link* head;
  struct link* tail;
} list;

typedef struct link {
  struct link* prev;
  struct link* next;
} link;

void list_init(list* l); 
size_t list_length(list* l);

#ifdef __cplusplus
}
#endif
#endif /* defined(LIST_H) */
