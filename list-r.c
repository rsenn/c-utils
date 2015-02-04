#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#ifndef _WIN32
#include <unistd.h>
#include <dirent.h>
#else
#include <io.h>
#endif
#include <string.h>
#include <sys/stat.h>

#include "buffer.h"
#include "stralloc.h"
#include "fmt.h"
#include "dir_internal.h"

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MSYS__)
#include <windows.h>
#include <shlwapi.h>
#endif

static int opt_list = 0;
/*static char buffer_1_out[BUFFER_OUTSIZE];
static buffer buffer_1 = BUFFER_INIT((void*)write, 1, buffer_1_out, BUFFER_OUTSIZE);*/
int list_dir_internal(stralloc *dir,  char type);

int list_dir(stralloc *dir)
{
  //list_dir_internal(dir, DT_DIR);
  //list_dir_internal(dir, -DT_DIR);
return 0;
  }

int mode_str(stralloc *out, int mode) {
	stralloc_catb(&out, (mode & S_IRUSR) ? "r" : "-", 1);
	stralloc_catb(&out, (mode & S_IWUSR) ? "w" : "-", 1);
	stralloc_catb(&out, (mode & S_IXUSR) ? "x" : "-", 1);
	stralloc_catb(&out, (mode & S_IRGRP) ? "r" : "-", 1);
	stralloc_catb(&out, (mode & S_IWGRP) ? "w" : "-", 1);
	stralloc_catb(&out, (mode & S_IXGRP) ? "x" : "-", 1);
	stralloc_catb(&out, (mode & S_IROTH) ? "r" : "-", 1);
	stralloc_catb(&out, (mode & S_IWOTH) ? "w" : "-", 1);
	stralloc_catb(&out, (mode & S_IXOTH) ? "x" : "-", 1);
}


int list_dir_internal(stralloc *dir,  char type)
{
  size_t l;
  struct dir_s d;
	stralloc pre;
	int dtype;
  int is_dir, is_symlink;
  unsigned long len;
#ifndef _WIN32
  struct stat st;
#endif
  
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

#if !defined(_WIN32)
    if(lstat(dir->s, &st) != -1)
      is_symlink = S_ISLNK(st.st_mode);
    else
#endif
      is_symlink = 0;
	
#if !defined(_WIN32)
    struct stat st;
    if(lstat(dir->s, &st) != -1)
      is_symlink = !!S_ISLNK(st.st_mode);
    else
#endif
      is_symlink = 0;

		dtype = dir_type(&d); 

		if(dtype) {
			is_dir= !!(dtype & D_DIRECTORY);
		} else {
#if defined(_WIN32)
			is_dir = 0;
#else
			is_dir = !!S_ISDIR(st.st_mode);
#endif
		}

   if(opt_list) {
		 stralloc_init(&pre);
     mode_str(&pre, st.st_mode);
		 stralloc_catb(&pre, " ", 1);
		 stralloc_catulong0(&pre, st.st_uid, FMT_LONG);
		 stralloc_catb(&pre, " ", 1);
	 }

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

   if(opt_list)
     buffer_putsa(buffer_1, &pre);

    buffer_put(buffer_1, s,len);
    buffer_put(buffer_1, "\n", 1);
    buffer_flush(buffer_1);

    if(is_dir && !is_symlink)
    {
      dir->len--;
      list_dir_internal(dir,0);
    }
  }
  dir_close(&d);
  return 0;
}

int main(int argc, char *argv[]) {

  stralloc dir= {0,0,0};
	int argi = 1;

  if(argc > 1) {
		if(!strcmp(argv[argi], "-l")) {
			opt_list = 1;
			argi++;
		}
	}

  if(argi < argc) {
		
		while(argi < argc) {
			stralloc_copys(&dir,argv[argi]);
      list_dir_internal(&dir,0);
			argi++;
		}
	} else { 
			stralloc_copys(&dir,".");
      list_dir_internal(&dir,0);
	}
  return 0;


}
