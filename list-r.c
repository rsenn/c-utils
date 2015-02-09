#define _LARGEFILE_SOURCE 1
#define _GNU_SOURCE 1
#define _FILE_OFFSET_BITS 64

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
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#include "buffer.h"
#include "stralloc.h"
#include "fmt.h"
#include "uint64.h"
#include "dir_internal.h"

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MSYS__)
#include <windows.h>
#include <shlwapi.h>
#endif

static int opt_list = 0, opt_numeric = 0;
static const char* opt_timestyle = "%b %2e %H:%M";

int
list_dir_internal(stralloc *dir,  char type);

static void
make_num(stralloc *out, unsigned long num, size_t width) {
	char fmt[FMT_ULONG+1];
  size_t sz = fmt_ulong(fmt, num);
  
	int n = width - sz; 

	while(n-- > 0) {
		stralloc_catb(out, " ", 1);
	}
	stralloc_catb(out, fmt, sz);
}

static void
make_time(stralloc *out, time_t t, size_t width) {
	if(opt_numeric) {
		 make_num(out, (unsigned long)t, width);
	} else {
		struct tm ltime;
    char buf[1024];
		size_t sz; 
	  int n; 	
		localtime_r(&t, &ltime);
		sz = strftime(buf, sizeof(buf), opt_timestyle , &ltime);
		n = width - sz;
		while(n-- > 0) {
			stralloc_catb(out, " ", 1);
		}
		stralloc_catb(out, buf, sz);
	}
}

static void
mode_str(stralloc *out, int mode) {
	char mchars[10];
	switch(mode & S_IFMT) {
#ifdef S_IFLNK
		case S_IFLNK: mchars[0] = 'l'; break;
#endif
		case S_IFDIR: mchars[0] = 'd'; break;
		case S_IFCHR: mchars[0] = 'c'; break;
		case S_IFBLK: mchars[0] = 'b'; break;
		case S_IFIFO: mchars[0] = 'i'; break;
#ifdef S_IFSOCK
		case S_IFSOCK: mchars[0] = 's'; break;
#endif
		case S_IFREG: 
		default: mchars[0] = '-'; break;
	}
#ifdef S_IRUSR
	if(mode & S_IRUSR) mchars[1] = 'r'; else
#endif
	mchars[1] = '-';

#ifdef S_IWUSR
	if(mode & S_IWUSR) mchars[2] = 'w'; else
#endif
	mchars[2] = '-';

#ifdef S_IXUSR
	if(mode & S_IXUSR) mchars[3] = 'x'; else
#endif
	mchars[3] = '-';

#ifdef S_IRGRP
	if(mode & S_IRGRP) mchars[4] = 'r'; else
#endif
	mchars[4] = '-';

#ifdef S_IWGRP
	if(mode & S_IWGRP) mchars[5] = 'w'; else
#endif
	mchars[5] = '-';

#ifdef S_IXGRP
	if(mode & S_IXGRP) mchars[6] = 'x'; else
#endif
	mchars[6] = '-';

#ifdef S_IROTH
	if(mode & S_IROTH) mchars[7] = 'r'; else
#endif
	mchars[7] = '-';

#ifdef S_IWOTH
	if(mode & S_IWOTH) mchars[8] = 'w'; else
#endif
	mchars[8] = '-';

#ifdef S_IXOTH
	if(mode & S_IXOTH) mchars[9] = 'x'; else
#endif
	mchars[9] = '-';

	stralloc_catb(out, mchars, sizeof(mchars));
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
	  unsigned int mode = 0, nlink = 0, uid = 0, gid = 0;
		uint64 size = 0, mtime = 0;

    dir->len = l;

    if(strcmp(name, "") == 0) continue;
    if(strcmp(name, ".") == 0) continue;
    if(strcmp(name, "..") == 0) continue;

    stralloc_readyplus(dir, strlen(name)+1);
    strcpy(dir->s + dir->len, name);
    dir->len+=strlen(name);

#if !defined(_WIN32)

    if(lstat(dir->s, &st) != -1)
      is_symlink = S_ISLNK(mode);
    else
#endif
      is_symlink = 0;
	
#if !defined(_WIN32)
    struct stat st;
    if(lstat(dir->s, &st) != -1)
      is_symlink = !!S_ISLNK(mode);
    else
#endif
      is_symlink = 0;

		dtype = dir_type(&d); 

#ifndef _WIN32
    mode = st.st_mode;
    nlink = st.st_nlink;
    uid = st.st_uid;
    gid = st.st_gid;
    size = st.st_size;
    mtime = st.st_mtime;
#endif

		if(dtype) {
			is_dir= !!(dtype & D_DIRECTORY);
		} else {
#if defined(_WIN32)
			is_dir = 0;
#else
			is_dir = !!S_ISDIR(mode);
#endif
		}

   if(opt_list) {
		 stralloc_init(&pre);
     mode_str(&pre, mode);
		 stralloc_catb(&pre, " ", 1);
		 make_num(&pre, nlink, 3);
		 stralloc_catb(&pre, " ", 1);
		 make_num(&pre, uid, 0);
		 stralloc_catb(&pre, " ", 1);
		 make_num(&pre, gid, 0);
		 stralloc_catb(&pre, " ", 1);
		 make_num(&pre, size, 6);
		 stralloc_catb(&pre, " ", 1);
		 make_time(&pre, mtime, 10);
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

  while(argi < argc) {
		if(!strcmp(argv[argi], "-l") || !strcmp(argv[argi], "--list")) {
			opt_list = 1;
		} else if(!strcmp(argv[argi], "-n") || !strcmp(argv[argi], "--numeric")) {
			opt_numeric = 1;
		} else if(!strcmp(argv[argi], "-t") || !strcmp(argv[argi], "--time-style")) {
			argi++;
			opt_timestyle = argv[argi];
		} else {
			break;
		}
		argi++;
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
