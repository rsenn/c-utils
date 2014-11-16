#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "buffer.h"
#include "stralloc.h"
#include "dir_internal.h"

#if defined(__MINGW32__) || defined(__MSYS__)
#include <windows.h>
#include <shlwapi.h>
#endif

static char buffer_1_out[BUFFER_OUTSIZE];
static buffer buffer_1 = BUFFER_INIT((void*)write, 1, buffer_1_out, BUFFER_OUTSIZE);
int list_dir_internal(stralloc *dir,  char type);

int list_dir(stralloc *dir)
{
  //list_dir_internal(dir, DT_DIR);
  //list_dir_internal(dir, -DT_DIR);
return 0;
  }

int list_dir_internal(stralloc *dir,  char type)
{
  size_t l;
  struct dir_s d;
  int is_dir, is_symlink;
  unsigned long len;
  struct stat st;
  
  char *name,*s;

  while(dir->len > 0 && IS_PATHSEP(dir->s[dir->len - 1]))
    dir->len--;
  
  stralloc_nul(dir);
  
  dir_open(&d, dir->s);
  
  
  stralloc_cats(dir, PATHSEP_S);
  l = dir->len;
  
  
  while((name = dir_read(&d)))
  {
    dir->len = l;

    if(strcmp(name, "") == 0) continue;
    if(strcmp(name, ".") == 0) continue;
    if(strcmp(name, "..") == 0) continue;

    stralloc_readyplus(dir, strlen(name)+1);
    strcpy(dir->s + dir->len, name);
    dir->len+=strlen(name);

    if(lstat(dir->s, &st) != -1)
      is_symlink = S_ISLNK(st.st_mode);
    else
      is_symlink = 0;
	
#if !defined(__MINGW32__)
    struct stat st;
    if(lstat(dir->s, &st) != -1)
      is_symlink = !!S_ISLNK(st.st_mode);
    else
#endif
      is_symlink = 0;


    is_dir= !!(dir_type(&d) & D_DIRECTORY);

	//fprintf(stderr,"%d %08x\n", is_dir, dir_ATTRS(&d));
    if(is_dir)
      stralloc_cats(dir, PATHSEP_S);
    
	s=dir->s;
    len=dir->len;
    if(len >= 2 && s[0] == '.' && IS_PATHSEP(s[1]))
    {
      len -= 2;
      s+= 2;
    }

    buffer_put(&buffer_1, s,len);
    buffer_put(&buffer_1, "\n", 1);
    buffer_flush(&buffer_1);

    if(is_dir && !is_symlink)
    {
      dir->len--;
      list_dir_internal(dir,0);
    }
  }
  dir_close(&d);
  return 0;
}

int main(int argc, char *argv[])
{
  stralloc dir= {0,0,0};
  if(argc > 1)
  stralloc_copys(&dir,argv[1]);
  else stralloc_copys(&dir,".");
  list_dir_internal(&dir,0);
  return 0;
}
