/* this header file comes from libowfat, http://www.fefe.de / libowfat/ */
#ifndef MMAP_H
#define MMAP_H

//#include <stddef.h>
#include "uint64.h"

#ifdef _MSC_VER
#include <windows.h>
# ifdef _WIN64
typedef signed __int64 ssize_t;
# else
typedef signed __int32 ssize_t;
# endif
#endif


#ifdef __cplusplus
extern "C" {
#endif

/* open file for reading, mmap whole file, close file, write length of
 * map in filesize and return pointer to map. */
char* mmap_read(const char* filename, size_t* filesize);
char* mmap_read_fd(int fd, size_t* filesize);

/* open file for writing, mmap whole file privately (copy on write),
 * close file, write length of map in filesize and return pointer to
 * map. */
char* mmap_private(const char* filename, size_t* filesize);

/* open file for writing, mmap whole file shared, close file, write
 * length of map in filesize and return pointer to map. */
char* mmap_shared(const char* filename, size_t* filesize);

/* unmap a mapped region */
int mmap_unmap(char* mapped, size_t maplen);

char* mmap_map(int fd, size_t sz, uint64 offset);


#ifdef __cplusplus
}
#endif

#endif
