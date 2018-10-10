#ifndef _DIR_H__
#define _DIR_H__

#ifdef __cplusplus
extern "C" {
#endif

#if (defined(_WIN32) || defined(_WIN64)) && !defined(__MSYS__) && !defined(__CYGWIN__)
#define PATHSEP_C '\\'
#define PATHSEP_S "\\"
#define IS_PATHSEP(c) ((c) == '\\'||(c) == '/')
#else
#define PATHSEP_C '/'
#define PATHSEP_S "/"
#define IS_PATHSEP(c) ((c) == '/')
#endif

enum
{
  D_FILE = 0x20,
  D_DIRECTORY = 0x10,
  D_SYMLINK = 0x08,
};

enum
{
  D_TIME_CREATION,
  D_TIME_ACCESS,
  D_TIME_MODIFICATION,
};

typedef struct dir_s { void* dir_int; } dir_t;

int dir_open(struct dir_s* d, const char* p);
char* dir_read(struct dir_s* d);
void dir_close(struct dir_s* d);

char* dir_name(struct dir_s* d);
int dir_type(struct dir_s* d);
//unsigned long dir_time(struct dir_s* d, int time_type);

#ifdef __cplusplus
}
#endif

#endif /* _DIR_H__ */
/* dir_time.c */
