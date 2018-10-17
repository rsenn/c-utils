/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef MMAP_H
#define MMAP_H

#include "typedefs.h"
#include "uint64.h"

#ifdef __cplusplus
extern "C" {
#endif

/* map fd
 * return pointer to map */
char *mmap_map(intptr_t fd, size_t sz, uint64 offset);

/* map fd for reading
 * return pointer to map */
char *mmap_read_fd(intptr_t fd, size_t *filesize);

/* open file for reading, mmap whole file, close file, write length of
 * map in filesize and return pointer to map. */
char* mmap_read(const char *filename,size_t* filesize);

/* open file for writing, mmap whole file privately (copy on write),
 * close file, write length of map in filesize and return pointer to
 * map. */
char* mmap_private(const char *filename,size_t* filesize);

char* mmap_private_fd(intptr_t fd, size_t* filesize);

/* open file for writing, mmap whole file shared, close file, write
 * length of map in filesize and return pointer to map. */
char* mmap_shared(const char *filename,size_t* filesize);

char* mmap_shared_fd(intptr_t fd,size_t* filesize);

/* unmap a mapped region */
int mmap_unmap(void* mapped, size_t maplen);

#ifdef STRALLOC_H
int mmap_filename(void* map, stralloc* sa);
#endif

#ifdef __cplusplus
}
#endif

#endif