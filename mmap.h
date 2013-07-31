#ifndef MMAP_H
#define MMAP_H

char* mmap_read_fd(int fd, unsigned long *filesize);

/* open file for reading, mmap whole file, close file, write length of
 * map in filesize and return pointer to map. */
char* mmap_read(const char *filename,unsigned long* filesize);

/* open file for writing, mmap whole file privately (copy on write),
 * close file, write length of map in filesize and return pointer to
 * map. */
char* mmap_private(const char *filename,unsigned long* filesize);

/* open file for writing, mmap whole file shared, close file, write
 * length of map in filesize and return pointer to map. */
char* mmap_shared(const char *filename,unsigned long* filesize);

#endif
