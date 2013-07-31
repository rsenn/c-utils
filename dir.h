#ifndef _DIR_H__
#define _DIR_H__

#if defined(__MINGW32__) || defined(__MSYS__)
#define PATHSEP_C '\\'
#define PATHSEP_S "\\"
#else
#define PATHSEP_C '/'
#define PATHSEP_S "/"
#endif
#define IS_PATHSEP(c) ((c)=='\\'||(c)=='/')

enum 
{
  D_FILE = 0x20,
  D_DIRECTORY = 0x10,
};

struct dir;

int dir_open(struct dir *d, const char *p);
char *dir_read(struct dir *d);
void dir_close(struct dir *d);

int dir_type(struct dir *d);

#endif // _DIR_H__
