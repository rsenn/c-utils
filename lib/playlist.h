#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <stdlib.h>
#include <sys/types.h>

#include "stralloc.h"
#include "uint32.h"
#include "buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

struct playlist;

typedef void (playlist_callback)(struct playlist*, stralloc* title, stralloc* path, uint32 len);
typedef playlist_callback* playlist_callback_ptr;

typedef enum playlist_type {
  UNKNOWN = 0,
  M3U = 1,
  XSPF = 2,
  PLS = 3,
} playlist_type;

typedef struct playlist_entry {
  stralloc title;
  stralloc path;
  uint32 length;
} playlist_entry;

typedef struct playlist {
  playlist_entry* entry;
  playlist_type type;
  playlist_callback_ptr callback;
  int (*reader)(struct playlist *);
  void *ptr;
} playlist;

void playlist_init(playlist *pl);
void playlist_xspf(playlist *pls, buffer *b);

int playlist_read(playlist *pl);

#ifdef __cplusplus
}
#endif
#endif /* defined(PLAYLIST_H) */
