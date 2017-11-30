#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <stdlib.h>
#include <sys/types.h>

#include "stralloc.h"
#include "uint32.h"
#include "buffer.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct playlist_entry {
  stralloc title;
  stralloc path;
  uint32 length; 
} playlist_entry;

typedef struct playlist {
  playlist_entry* entry;
  void *ptr;
} playlist;

void playlist_init(playlist *pl);
void playlist_xspf(playlist *pls, buffer *b);

#ifdef __cplusplus
}
#endif
#endif /* defined(PLAYLIST_H) */
