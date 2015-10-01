#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#define _LARGEFILE_SOURCE 1
#define _GNU_SOURCE 1
#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <stdlib.h>

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#endif
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#ifndef _WIN32
#include <unistd.h>
#include <dirent.h>
#else
#include <io.h>
#include <aclapi.h>
#include <sddl.h>
#include <winternl.h>
#include <wtypes.h>
#endif
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#include "buffer.h"
#include "stralloc.h"
#include "fmt.h"
#include "str.h"
#include "byte.h"
#include "uint64.h"
#include "dir_internal.h"

#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MSYS__)
#define PLAIN_WINDOWS 1
#endif

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MSYS__)
#include <windows.h>
#include <shlwapi.h>
#endif

static int opt_list = 0, opt_numeric = 0;
static const char* opt_timestyle = "%b %2e %H:%M";

static INLINE uint64_t filetime_to_unix(const FILETIME* ft);
#ifdef _WIN32

static const char*
last_error_str () {
  DWORD errCode = GetLastError();
  static char buffer[1024];
  char *err;
  buffer[0] = '\0';
  if(errCode == 0) return buffer;
  
   SetLastError(0);
  if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
					 NULL,
					 errCode,
					 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
					 (LPTSTR) &err,
					 0,
					 NULL))
	  return 0;

  
  snprintf(buffer, sizeof(buffer), "ERROR: %s\n", err);
  
  //OutputDebugString(buffer); // or otherwise log it
  LocalFree(err);
  return buffer;
}

int64 get_file_size(char* path) {
  LARGE_INTEGER size;
  HANDLE hFile = CreateFileA(path, GENERIC_READ, 
	  FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 
	  FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile==INVALID_HANDLE_VALUE)
	  return -1; // error condition, could call GetLastError to find out more

  if (!GetFileSizeEx(hFile, &size))
  {
	  CloseHandle(hFile);
	  return -1; // error condition, could call GetLastError to find out more
  }

  CloseHandle(hFile);

//  fprintf(stderr, "get_file_size: %s = %"PRIi64" [%s]\n", path, (int64)size.QuadPart, last_error_str());  
  return size.QuadPart;
}	

uint64_t get_file_time(const char* path) {
  FILETIME c, la, lw;
  int64 t;
 HANDLE hFile = CreateFileA(path, GENERIC_READ, 
	  FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 
	  FILE_ATTRIBUTE_NORMAL, NULL);
  if(hFile == INVALID_HANDLE_VALUE)
	  return -1; // error condition, could call GetLastError to find out more

  if(!GetFileTime(hFile, &c, &la, &lw)) {
	  CloseHandle(hFile);
	  return -1; // error condition, could call GetLastError to find out more
  }

  CloseHandle(hFile);

  if((t = filetime_to_unix(&lw)) <= 0)
    if((t = filetime_to_unix(&c)) <= 0)
      t = filetime_to_unix(&la);

//  fprintf(stderr, "get_file_size: %s = %"PRIi64" [%s]\n", path, (int64)size.QuadPart, last_error_str());  
  return t;
}

const char*
get_file_owner(const char* path) {
  static char buffer[1024];
  DWORD dwRtnCode = 0;
  PSID pSidOwner = NULL;
  BOOL bRtnBool = TRUE;
  LPTSTR AcctName = NULL;
  LPTSTR DomainName = NULL;
  DWORD dwAcctName = 1, dwDomainName = 1;
  SID_NAME_USE eUse = SidTypeUnknown;
  HANDLE hFile;
  PSECURITY_DESCRIPTOR pSD = NULL;
LPSTR strsid = NULL;

  buffer[0] = '\0';

  // Get the handle of the file object.
  hFile = CreateFileA(path,
					GENERIC_READ,
					FILE_SHARE_READ,
					NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					NULL);

  // Check GetLastError for CreateFile error code.
  if (hFile == INVALID_HANDLE_VALUE) {
			DWORD dwErrorCode = 0;

			dwErrorCode = GetLastError();
//			snprintf(buffer, sizeof(buffer), "CreateFile error = %d\n", dwErrorCode);
			return  0;
  }



  // Get the owner SID of the file.
  dwRtnCode = GetSecurityInfo(
					hFile,
					SE_FILE_OBJECT,
					OWNER_SECURITY_INFORMATION,
					&pSidOwner,
					NULL,
					NULL,
					NULL,
					&pSD);

  // Check GetLastError for GetSecurityInfo error condition.
  if (dwRtnCode != ERROR_SUCCESS) {
			DWORD dwErrorCode = 0;

			dwErrorCode = GetLastError();
	//		snprintf(buffer, sizeof(buffer), "GetSecurityInfo error = %d\n", dwErrorCode);
			return 0;
  }
  
  
  if(ConvertSidToStringSid(pSidOwner, &strsid)) {
    snprintf(buffer,sizeof(buffer), "%s", strsid);
    LocalFree(strsid);
  }

  // First call to LookupAccountSid to get the buffer sizes.
  bRtnBool = LookupAccountSid(
					NULL,           // local computer
					pSidOwner,
					AcctName,
					(LPDWORD)&dwAcctName,
					DomainName,
					(LPDWORD)&dwDomainName,
					&eUse);

  // Reallocate memory for the buffers.
  AcctName = (LPTSTR)GlobalAlloc(
			GMEM_FIXED,
			dwAcctName);

  // Check GetLastError for GlobalAlloc error condition.
  if (AcctName == NULL) {
			DWORD dwErrorCode = 0;

			dwErrorCode = GetLastError();
		//	snprintf(buffer, sizeof(buffer), "GlobalAlloc error = %d\n", dwErrorCode);
			return buffer;
  }

  DomainName = (LPTSTR)GlobalAlloc(
		 GMEM_FIXED,
		 dwDomainName);

  // Check GetLastError for GlobalAlloc error condition.
  if (DomainName == NULL) {
		DWORD dwErrorCode = 0;

		dwErrorCode = GetLastError();
		//snprintf(buffer, sizeof(buffer), "GlobalAlloc error = %d\n", dwErrorCode);
		return buffer;

  }

  // Second call to LookupAccountSid to get the account name.
  bRtnBool = LookupAccountSid(
		NULL,                   // name of local or remote computer
		pSidOwner,              // security identifier
		AcctName,               // account name buffer
		(LPDWORD)&dwAcctName,   // size of account name buffer 
		DomainName,             // domain name
		(LPDWORD)&dwDomainName, // size of domain name buffer
		&eUse);                 // SID type

  // Check GetLastError for LookupAccountSid error condition.
  if(bRtnBool == FALSE) {
		DWORD dwErrorCode = 0;

		dwErrorCode = GetLastError();

		if(dwErrorCode == ERROR_NONE_MAPPED)
			snprintf(buffer, sizeof(buffer), "Account owner not found for specified SID.\n");
		else 
			snprintf(buffer, sizeof(buffer), "Error in LookupAccountSid.\n");
		return buffer;

  } else if (bRtnBool == TRUE) 
	  // Print the account name.
	  snprintf(buffer, sizeof(buffer), "%s", AcctName);
	  return buffer;
  }

#endif

#ifdef PLAIN_WINDOWS


#define WINDOWS_TICK 10000000
#define SEC_TO_UNIX_EPOCH 11644473600LL

static INLINE uint64_t
filetime_to_unix(const FILETIME* ft) {
  uint64_t windowsTicks = ((uint64_t)ft->dwHighDateTime << 32) + ft->dwLowDateTime;
  return (uint64_t)(windowsTicks / 10000000 - SEC_TO_UNIX_EPOCH);
}

int
is_junction_point(const char* fn) {
  int status = 0;

  WIN32_FIND_DATA FindFileData;
  HANDLE hFind;

  hFind = FindFirstFile(fn, &FindFileData);
  if(INVALID_HANDLE_VALUE != hFind) {
    if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
      // We're probably going to skip this reparse point,
      // but not always. (See the logic below.)
      status = 1;

      // Tag values come from
      // http://msdn.microsoft.com/en-us/library/dd541667(prot.20).aspx

      switch(FindFileData.dwReserved0) {
      case IO_REPARSE_TAG_MOUNT_POINT:
        //ocb.error_filename(fn, "Junction point, skipping");
        break;

#ifdef IO_REPARSE_TAG_SYMLINK
      case IO_REPARSE_TAG_SYMLINK:
        // TODO: Maybe have the option to follow symbolic links?
       // ocb.error_filename(fn, "Symbolic link, skipping");
        break;
#endif

      // TODO: Use label for deduplication reparse point
      //         when the compiler supports it
      //      case IO_REPARSE_TAG_DEDUP:
      case 0x80000013:
        // This is the reparse point for Data Deduplication
        // See http://blogs.technet.com/b/filecab/archive/2012/05/21/introduction-to-data-deduplication-in-windows-server-2012.aspx
        // Unfortunately the compiler doesn't have this value defined yet.
        status = 0;
        break;

      case IO_REPARSE_TAG_SIS:
        // Single Instance Storage
        // "is a system's ability to keep one copy of content that multiple users or computers share"
        // http://blogs.technet.com/b/filecab/archive/2006/02/03/single-instance-store-sis-in-windows-storage-server-r2.aspx
        status = 0;
        break;

      default:
        break;
      }
    }

    // We don't error check this call as there's nothing to do differently
    // if it fails.
    FindClose(hFind);
  }

  return status;
}
#endif

static int
list_dir_internal(stralloc* dir,  char type);

static void
make_str(stralloc* out, const char* s, size_t width) {
  size_t sz = str_len(s);
  if(sz < width)  {
	ssize_t n = width - sz;
	while(n-- > 0) {
	  stralloc_catb(out, " ", 1);
	}
  }
  stralloc_catb(out, s, sz);
}

static void
make_num(stralloc* out, int64 num, size_t width) {
  char fmt[FMT_LONG*2 + 1];
  int i = 0;  
  size_t sz = fmt_int64(&fmt[i], num);

  ssize_t n = width - sz;

  while(n-- > 0) {
    stralloc_catb(out, " ", 1);
  }
  stralloc_catb(out, fmt, sz);
}

void
make_time(stralloc* out, time_t t, size_t width) {
  if(opt_numeric) {
    make_num(out, (size_t)t, width);
  } else {
    struct tm ltime;
    char buf[1024];
    size_t sz;
    ssize_t n;
#ifdef HAVE_LOCALTIME_R_FUNC
    localtime_r(&t, &ltime);
#else
    ltime = *localtime(&t);
#endif
    sz = strftime(buf, sizeof(buf), opt_timestyle , &ltime);
    n = width - sz;
    while(n-- > 0) {
      stralloc_catb(out, " ", 1);
    }
    stralloc_catb(out, buf, sz);
  }
}

static void
mode_str(stralloc* out, int mode) {
  char mchars[10];
  switch(mode & S_IFMT) {
#ifdef S_IFLNK
  case S_IFLNK:
    mchars[0] = 'l';
    break;
#endif
  case S_IFDIR:
    mchars[0] = 'd';
    break;
  case S_IFCHR:
    mchars[0] = 'c';
    break;
#ifdef S_IFBLK
  case S_IFBLK:
    mchars[0] = 'b';
    break;
#endif
#ifdef S_IFIFO
  case S_IFIFO:
    mchars[0] = 'i';
    break;
#endif
#ifdef S_IFSOCK
  case S_IFSOCK:
    mchars[0] = 's';
    break;
#endif
  case S_IFREG:
  default:
    mchars[0] = '-';
    break;
  }
#ifdef S_IRUSR
  if(mode & S_IRUSR) mchars[1] = 'r';
  else
#endif
    mchars[1] = '-';

#ifdef S_IWUSR
  if(mode & S_IWUSR) mchars[2] = 'w';
  else
#endif
    mchars[2] = '-';

#ifdef S_IXUSR
  if(mode & S_IXUSR) mchars[3] = 'x';
  else
#endif
    mchars[3] = '-';

#ifdef S_IRGRP
  if(mode & S_IRGRP) mchars[4] = 'r';
  else
#endif
    mchars[4] = '-';

#ifdef S_IWGRP
  if(mode & S_IWGRP) mchars[5] = 'w';
  else
#endif
    mchars[5] = '-';

#ifdef S_IXGRP
  if(mode & S_IXGRP) mchars[6] = 'x';
  else
#endif
    mchars[6] = '-';

#ifdef S_IROTH
  if(mode & S_IROTH) mchars[7] = 'r';
  else
#endif
    mchars[7] = '-';

#ifdef S_IWOTH
  if(mode & S_IWOTH) mchars[8] = 'w';
  else
#endif
    mchars[8] = '-';

#ifdef S_IXOTH
  if(mode & S_IXOTH) mchars[9] = 'x';
  else
#endif
    mchars[9] = '-';

  stralloc_catb(out, mchars, sizeof(mchars));
}


int list_dir_internal(stralloc* dir,  char type) {
  size_t l;
  struct dir_s d;
  stralloc pre;
  int dtype;
  int is_dir, is_symlink;
  size_t len;
#ifdef USE_LSTAT
  struct stat st;
  static dev_t root_dev;
#endif
  char *name, *s;
const char* u;

  (void)type;

  while(dir->len > 0 && IS_PATHSEP(dir->s[dir->len - 1]))
    dir->len--;

  stralloc_nul(dir);

#ifndef PLAIN_WINDOWS
  if(root_dev == 0) {
    if(stat(dir->s, &st) != -1) {
      root_dev = st.st_dev;
    }
  }
#endif

  dir_open(&d, dir->s);

  stralloc_cats(dir, PATHSEP_S);
  l = dir->len;

  while((name = dir_read(&d))) {
    unsigned int mode = 0, nlink = 0, uid = 0, gid = 0;
    char user[32], group[32];
    int64 size = 0, mtime = 0;
    
    user[0] = '\0';
    group[0] = '\0';

    dir->len = l;

    if(strcmp(name, "") == 0 || strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
      continue;
    }

    stralloc_readyplus(dir, strlen(name) + 1);
    strcpy(dir->s + dir->len, name);
    dir->len += strlen(name);

#ifdef USE_LSTAT
    if(lstat(dir->s, &st) != -1) {
      if(root_dev && st.st_dev) {
        if(st.st_dev != root_dev) {
          continue;
        }
      }
      is_symlink = !!S_ISLNK(mode);
    } else
# endif
      is_symlink = 0;

    dtype = dir_type(&d);


    if (dtype) {
      is_dir = !!(dtype & D_DIRECTORY);
    } else {
# ifndef USE_LSTAT
      is_dir = 0;
# else
      is_dir = !!S_ISDIR(mode);
# endif
    }

    if(dtype & D_SYMLINK)
      is_symlink = 1;

/*    if(is_dir) {
      stralloc_cats(dir, PATHSEP_S);
      dir->len--;
      }*/


#ifdef USE_LSTAT //ndef PLAIN_WINDOWS
    mode = st.st_mode;
    nlink = st.st_nlink;
    uid = st.st_uid;
    gid = st.st_gid;
    size = st.st_size;
    mtime = st.st_mtime;
#else
    mode = (is_dir ? S_IFDIR : (is_symlink ? S_IFLNK : S_IFREG));
    
# ifdef USE_READDIR
  if(!is_dir) {
	 size = get_file_size(s); // dir_INTERNAL(&d)->dir_entry->d_name);
	 mtime = get_file_time(s);
	 
   } else {
	 mtime = 0;
	 size = 0;
	  
   }
# else
    size = ((uint64_t)(dir_INTERNAL(&d)->dir_finddata.nFileSizeHigh) << 32) + dir_INTERNAL(&d)->dir_finddata.nFileSizeLow;
    mtime = filetime_to_unix(&dir_INTERNAL(&d)->dir_finddata.ftLastWriteTime);
# endif

#endif
    
   
   if(gid != 0) fmt_long(group, gid);
   else byte_copy(group, 2, "-");
   
   /*if((u = get_file_owner(s))) byte_copy(user, str_len(u), u);
   else*/ if(uid != 0) fmt_long(user, uid);
   else byte_copy(user, 2, "-");
   
    //mtime = FileTime_to_POSIX(dir_INTERNAL(&d)->dir_finddata.ftLastWriteTime);


    if(opt_list) {
      stralloc_init(&pre);

      // Mode string
      mode_str(&pre, mode);
      stralloc_catb(&pre, " ", 1);
      // num links
      make_num(&pre, nlink, 3);
      stralloc_catb(&pre, " ", 1);
      // uid
      make_str(&pre, user, 10);
      stralloc_catb(&pre, " ", 1);
      // gid
      make_str(&pre, group, 10);
      stralloc_catb(&pre, " ", 1);
      // size
      make_num(&pre, size, 6);
      stralloc_catb(&pre, " ", 1);
      // time
      make_num(&pre, mtime, 10);

//     make_time(&pre, mtime, 10);
      stralloc_catb(&pre, " ", 1);
    }

    //fprintf(stderr, "%d %08x\n", is_dir, dir_ATTRS(&d));

    if(dir->len > PATH_MAX) {
      buffer_puts(buffer_2, "ERROR: Directory ");
      buffer_putsa(buffer_2, dir);
      buffer_puts(buffer_2, " longer than PATH_MAX!\n");
      buffer_flush(buffer_2);
      goto end;
    }

    if(opt_list)
      buffer_putsa(buffer_1, &pre);

    if(is_dir) {
      stralloc_catb(&dir, PATHSEP_S, 1);
      dir->len -= 1;
    }

   stralloc_catb(&dir, "\0", 1);
   dir->len -= 1;

    s = dir->s;
    len = dir->len;
    if(len >= 2 && s[0] == '.' && IS_PATHSEP(s[1])) {
      len -= 2;
      s += 2;
    }

    buffer_put(buffer_1, s, len);
    buffer_put(buffer_1, "\n", 1);
    buffer_flush(buffer_1);

    if(is_dir && !is_symlink) {
  //    dir->len--;
      
      #ifndef NDEBUG
      fprintf(stderr, "recursion(%s,0)\n", s);
      #endif
      list_dir_internal(dir, 0);
    }
  }
end:
  dir_close(&d);
  return 0;
}

int main(int argc, char* argv[]) {

  stralloc dir = {0, 0, 0};
  int argi = 1;

  while(argi < argc) {
    if(!strcmp(argv[argi], "-l") || !strcmp(argv[argi], "--list")) {
      opt_list = 1;
    } else if(!strcmp(argv[argi], "-n") || !strcmp(argv[argi], "--numeric")) {
      opt_numeric = 1;
    } else if(!strcmp(argv[argi], "-t") || !strcmp(argv[argi], "--time - style")) {
      argi++;
      opt_timestyle = argv[argi];
    } else {
      break;
    }
    argi++;
  }

  if(argi < argc) {

    while(argi < argc) {
      stralloc_copys(&dir, argv[argi]);
      list_dir_internal(&dir, 0);
      argi++;
    }
  } else {
    stralloc_copys(&dir, ".");
    list_dir_internal(&dir, 0);
  }
  return 0;


}
