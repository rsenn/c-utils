/**
 * @defgroup   dir
 * @brief      DIR module.
 * @{
 */
#ifndef _DIR_H__
#define _DIR_H__

#include "uint64.h"

#ifdef __cplusplus
extern "C" {
#endif

#if(defined(_WIN32) || defined(_WIN64)) && !defined(__MSYS__) && \
    !defined(__CYGWIN__)
#define DIRSEP_C '\\'
#define DIRSEP_S "\\"
#define IS_DIRSEP(c) ((c) == '\\' || (c) == '/')
#else
#define DIRSEP_C '/'
#define DIRSEP_S "/"
#define IS_DIRSEP(c) ((c) == '/')
#endif

typedef enum dir_type {
  D_PIPE = 0x01,
  D_CHARDEV = 0x02,
  D_BLKDEV = 0x04,
  D_SYMLINK = 0x08,
  D_DIRECTORY = 0x10,
  D_FILE = 0x20,
  D_SOCKET = 0x40,
} dir_type_t;

enum {
  D_TIME_CREATION,
  D_TIME_ACCESS,
  D_TIME_MODIFICATION,
};

typedef struct dir_s {
  void* dir_int;
} dir_t;

int dir_open(struct dir_s* d, const char* p);
char* dir_read(struct dir_s* d);
void dir_close(struct dir_s* d);

char* dir_name(struct dir_s*);
int dir_type(struct dir_s*);
int64 dir_size(struct dir_s*);
unsigned long dir_time(struct dir_s*, int time_type);

#ifdef STRALLOC_H
void dir_path(struct dir_s* d, stralloc* sa);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _DIR_H__ */
/* dir_time.c */
/** @} */
