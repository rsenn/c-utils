/**
 * @defgroup   playlist
 * @brief      PLAYLIST module.
 * @{
 */
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

typedef void(playlist_callback)(struct playlist*,
                                stralloc* title,
                                stralloc* path,
                                uint32 len);
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
  int (*reader)(struct playlist*);
  void* ptr;
  size_t count;
  size_t num_items_pos;
} playlist;

typedef void playlist_type_fn(playlist*, buffer*);

void playlist_init(playlist* pl);

void playlist_xspf(playlist* pls, buffer* b);
void playlist_m3u(playlist* pls, buffer* b);
void playlist_pls(playlist* pls, buffer* b);

int playlist_read(playlist* pl);

int playlist_write_entry(buffer* b, playlist* pl, playlist_entry* e);
int playlist_write_start(buffer* b, playlist* pl);
int playlist_write_finish(buffer* b, playlist* pl);

#define playlist_settype(pl, t) \
  { (pl)->type = (t); }

#ifdef __cplusplus
}
#endif
#endif /* defined(PLAYLIST_H) */
/** @} */
