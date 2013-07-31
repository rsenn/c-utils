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

struct dir_s { void *dir_int; };

int dir_open(struct dir_s *d, const char *p);
char *dir_read(struct dir_s *d);
void dir_close(struct dir_s *d);

int dir_type(struct dir_s *d);

#endif // _DIR_H__
