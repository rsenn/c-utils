#define _LARGEFILE_SOURCE 1
#define _GNU_SOURCE 1
#define _FILE_OFFSET_BITS 64
#define INTERNAL_STRINGIFY(VALUE) #VALUE
#define STRINGIFY(VALUE) INTERNAL_STRINGIFY(VALUE)
#include <stdio.h>
#include <stdlib.h>
#ifdef _MSC_VER
#define snprintf _snprintf
#endif
#ifdef HAVE_STDBOOL_H
#endif
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#include "lib/windoze.h"
#include "lib/getopt.h"
#if !WINDOWS
#include <dirent.h>
#include <unistd.h>
#else
#include <aclapi.h>
#include <fcntl.h>
#include <io.h>
#include <sddl.h>
#include <wtypes.h>
#endif
#include "lib/array.h"
#include "lib/buffer.h"
#include "lib/dir_internal.h"
#include "lib/fmt.h"
#include "lib/fnmatch.h"
#include "lib/io_internal.h"
#include "lib/open.h"
#include "lib/str.h"
#include "lib/stralloc.h"
#include "lib/uint64.h"
#include "lib/ioctlcmd.h"
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#if WINDOWS
#include <io.h>
#include <shlwapi.h>
#include <windows.h>

#ifndef IO_REPARSE_TAG_SYMLINK
#define IO_REPARSE_TAG_SYMLINK 0xa000000c
#endif
#endif

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif
#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif
#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif

static void print_strarray(buffer* b, array* a);
static int fnmatch_strarray(buffer* b, array* a, const char* string, int flags);
static array exclude_masks;
static char opt_separator = DIRSEP_C;

static int opt_list = 0, opt_numeric = 0, opt_relative = 0;
static const char* opt_relative_to = 0;
static const char* opt_timestyle = "%b %2e %H:%M";
#if(defined(_WIN32) || defined(MINGW)) && !defined(__MSYS__)
static uint64 filetime_to_unix(const FILETIME* ft);
static const char*

last_error_str() {
  DWORD errCode = GetLastError();
  static char tmpbuf[1024];
  char* err;
  tmpbuf[0] = '\0';
  if(errCode == 0) return tmpbuf;
  SetLastError(0);
  if(!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                    0,
                    errCode,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), /* default language */
                    (LPTSTR)&err,
                    0,
                    0))
    return 0;
  snprintf(tmpbuf, sizeof(tmpbuf), "ERROR: %s\n", err);
  /* or otherwise log it */
  // OutputDebugString(tmpbuf);
  LocalFree(err);
  return tmpbuf;
}

int64
get_file_size(char* path) {
  LARGE_INTEGER size;
  HANDLE hFile = CreateFileA(
                   path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  if(hFile == INVALID_HANDLE_VALUE) return -1; /* error condition, could call GetLastError to find out more */
  if(!GetFileSizeEx(hFile, &size)) {
    CloseHandle(hFile);
    return -1; /* error condition, could call GetLastError to find out more */
  }
  CloseHandle(hFile);
  /*  fprintf(stderr, "get_file_size: %s = %"PRIi64" [%s]\n", path, (int64)size.QuadPart, last_error_str()); */
  return size.QuadPart;
}

uint64
get_file_time(const char* path) {
  FILETIME c, la, lw;
  int64 t;
  HANDLE hFile = CreateFileA(
                   path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  if(hFile == INVALID_HANDLE_VALUE) return -1; /* error condition, could call GetLastError to find out more */
  if(!GetFileTime(hFile, &c, &la, &lw)) {
    CloseHandle(hFile);
    return -1; /* error condition, could call GetLastError to find out more */
  }
  CloseHandle(hFile);
  if((t = filetime_to_unix(&lw)) <= 0)
    if((t = filetime_to_unix(&c)) <= 0) t = filetime_to_unix(&la);
  /*  fprintf(stderr, "get_file_size: %s = %"PRIi64" [%s]\n", path, (int64)size.QuadPart, last_error_str()); */
  return t;
}

const char*
get_file_owner(const char* path) {
  static char tmpbuf[1024];
  DWORD dwRtnCode = 0;
  PSID pSidOwner = 0;
  BOOL bRtnBool = TRUE;
  LPTSTR AcctName = 0;
  LPTSTR DomainName = 0;
  DWORD dwAcctName = 1, dwDomainName = 1;
  SID_NAME_USE eUse = SidTypeUnknown;
  HANDLE hFile;
  PSECURITY_DESCRIPTOR pSD = 0;
  LPSTR strsid = 0;
  DWORD dwErrorCode = 0;
  tmpbuf[0] = '\0';
  /* Get the handle of the file object. */
  hFile = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  /* Check GetLastError for CreateFile error code. */
  if(hFile == INVALID_HANDLE_VALUE) {
    dwErrorCode = GetLastError();
    /*   	snprintf(tmpbuf, sizeof(tmpbuf), "CreateFile error = %d\n", dwErrorCode); */
    return 0;
  }
  /* Get the owner SID of the file. */
  dwRtnCode = GetSecurityInfo(hFile, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, &pSidOwner, 0, 0, 0, &pSD);
  /* Check GetLastError for GetSecurityInfo error condition. */
  if(dwRtnCode != ERROR_SUCCESS) {
    dwErrorCode = GetLastError();
    /*   snprintf(tmpbuf, sizeof(tmpbuf), "GetSecurityInfo error = %d\n", dwErrorCode); */
    return 0;
  }
  if(ConvertSidToStringSid(pSidOwner, &strsid)) {
    snprintf(tmpbuf, sizeof(tmpbuf), "%s", strsid);
    LocalFree(strsid);
  }
  /* First call to LookupAccountSid to get the tmpbuf sizes. */
  bRtnBool = LookupAccountSid(0, /* local computer */
                              pSidOwner,
                              AcctName,
                              (LPDWORD)&dwAcctName,
                              DomainName,
                              (LPDWORD)&dwDomainName,
                              &eUse);
  /* Reallocate memory for the buffers. */
  AcctName = (LPTSTR)GlobalAlloc(GMEM_FIXED, dwAcctName);
  /* Check GetLastError for GlobalAlloc error condition. */
  if(AcctName == 0) {
    dwErrorCode = GetLastError();
    /* snprintf(tmpbuf, sizeof(tmpbuf), "GlobalAlloc error = %d\n", dwErrorCode); */
    return tmpbuf;
  }
  DomainName = (LPTSTR)GlobalAlloc(GMEM_FIXED, dwDomainName);
  /* Check GetLastError for GlobalAlloc error condition. */
  if(DomainName == 0) {
    dwErrorCode = GetLastError();
    /* snprintf(tmpbuf, sizeof(tmpbuf), "GlobalAlloc error = %d\n", dwErrorCode); */
    return tmpbuf;
  }
  /* Second call to LookupAccountSid to get the account name. */
  bRtnBool = LookupAccountSid(0,                   /* name of local or remote computer */
                              pSidOwner,              /* security identifier */
                              AcctName,               /* account name tmpbuf */
                              (LPDWORD)&dwAcctName,   /* size of account name tmpbuf */
                              DomainName,             /* domain name */
                              (LPDWORD)&dwDomainName, /* size of domain name tmpbuf */
                              &eUse);                 /* SID type */
  /* Check GetLastError for LookupAccountSid error condition. */
  if(bRtnBool == FALSE) {
    dwErrorCode = GetLastError();
    if(dwErrorCode == ERROR_NONE_MAPPED)
      snprintf(tmpbuf, sizeof(tmpbuf), "Account owner not found for specified SID.\n");
    else
      snprintf(tmpbuf, sizeof(tmpbuf), "Error in LookupAccountSid.\n");
    return tmpbuf;
  } else if(bRtnBool == TRUE)
    /* Print the account name. */
    snprintf(tmpbuf, sizeof(tmpbuf), "%s", AcctName);
  return tmpbuf;
}

#endif
#if WINDOWS_NATIVE
//#warning PLAIN_WINDOWS
#define WINDOWS_TICK 10000000
#define SEC_TO_UNIX_EPOCH (int64)11644473600

static uint64
filetime_to_unix(const FILETIME* ft) {
  uint64 windowsTicks = ((uint64)ft->dwHighDateTime << 32) + ft->dwLowDateTime;
  return (uint64)(windowsTicks / 10000000 - SEC_TO_UNIX_EPOCH);
}

int
is_junction_point(const char* fn) {
  int status = 0;
  WIN32_FIND_DATA FindFileData;
  HANDLE hFind;
  hFind = FindFirstFile(fn, &FindFileData);
  if(INVALID_HANDLE_VALUE != hFind) {
    if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
      /* We're probably going to skip this reparse point, */
      /* but not always. (See the logic below.) */
      status = 1;
      /* Tag values come from */
      /* http://msdn.microsoft.com/en-us/library/dd541667(prot.20).aspx */
      switch(FindFileData.dwReserved0) {
      case IO_REPARSE_TAG_MOUNT_POINT: /* ocb.error_filename(fn, "Junction point, skipping"); */ break;
      case IO_REPARSE_TAG_SYMLINK:
        /* TODO: Maybe have the option to follow symbolic links? */
        /* ocb.error_filename(fn, "Symbolic link, skipping"); */
        break;
      /* TODO: Use label for deduplication reparse point */
      /*         when the compiler supports it */
      /*      case IO_REPARSE_TAG_DEDUP: */
      case 0x80000013:
        /* This is the reparse point for Data Deduplication */
        /* See */
        /* http://blogs.technet.com/b/filecab/archive/2012/05/21/introduction-to-data-deduplication-in-windows-server-2012.aspx
         */
        /* Unfortunately the compiler doesn't have this value defined yet. */
        status = 0;
        break;
      case IO_REPARSE_TAG_SIS:
        /* Single Instance Storage */
        /* "is a system's ability to keep one copy of content that multiple users or computers share" */
        /* http://blogs.technet.com/b/filecab/archive/2006/02/03/single-instance-store-sis-in-windows-storage-server-r2.aspx
         */
        status = 0;
        break;
      default: break;
      }
    }
    /* We don't error check this call as there's nothing to do differently */
    /* if it fails. */
    FindClose(hFind);
  }
  return status;
}

#endif
static int list_dir_internal(stralloc* dir, char type);
static void
make_num(stralloc* out, size_t num, size_t width) {
  char fmt[FMT_ULONG + 1];
  size_t sz = fmt_uint64(fmt, num);
  ssize_t n = width - sz;
  while(n-- > 0) {
    stralloc_catb(out, " ", 1);
  }
  stralloc_catb(out, fmt, sz);
}

static void
print_strarray(buffer* b, array* a) {
  size_t i, n = array_length(a, sizeof(char *));
  char** x = array_start(a);
  for(i = 0; i < n; ++i) {
    char* s = x[i];
    if(s == 0) break;
    buffer_puts(b, x[i]);
    buffer_putc(b, '\n');
  }
  buffer_flush(b);
}

static int
fnmatch_strarray(buffer* b, array* a, const char* string, int flags) {
  size_t i, n = array_length(a, sizeof(char *));
  char** x = array_start(a);
  int ret = FNM_NOMATCH;
  for(i = 0; i < n; ++i) {
    char* s = x[i];
    if(s == 0) break;
    if((ret = fnmatch(s, string, flags)) != FNM_NOMATCH) break;
  }
  return ret;
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
    sz = strftime(buf, sizeof(buf), opt_timestyle, &ltime);
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
  case S_IFLNK: mchars[0] = 'l'; break;
#endif
  case S_IFDIR: mchars[0] = 'd'; break;
  case S_IFCHR: mchars[0] = 'c'; break;
#ifdef S_IFBLK
  case S_IFBLK: mchars[0] = 'b'; break;
#endif
#ifdef S_IFIFO
  case S_IFIFO: mchars[0] = 'i'; break;
#endif
#ifdef S_IFSOCK
  case S_IFSOCK: mchars[0] = 's'; break;
#endif
  case S_IFREG:
  default: mchars[0] = '-'; break;
  }
#ifdef S_IRUSR
  if(mode & S_IRUSR)
    mchars[1] = 'r';
  else
#endif
    mchars[1] = '-';

#ifdef S_IWUSR
  if(mode & S_IWUSR)
    mchars[2] = 'w';
  else
#endif
    mchars[2] = '-';

#ifdef S_IXUSR
  if(mode & S_IXUSR)
    mchars[3] = 'x';
  else
#endif
    mchars[3] = '-';

#ifdef S_IRGRP
  if(mode & S_IRGRP)
    mchars[4] = 'r';
  else
#endif
    mchars[4] = '-';

#ifdef S_IWGRP
  if(mode & S_IWGRP)
    mchars[5] = 'w';
  else
#endif
    mchars[5] = '-';

#ifdef S_IXGRP
  if(mode & S_IXGRP)
    mchars[6] = 'x';
  else
#endif
    mchars[6] = '-';

#ifdef S_IROTH
  if(mode & S_IROTH)
    mchars[7] = 'r';
  else
#endif
    mchars[7] = '-';

#ifdef S_IWOTH
  if(mode & S_IWOTH)
    mchars[8] = 'w';
  else
#endif
    mchars[8] = '-';

#ifdef S_IXOTH
  if(mode & S_IXOTH)
    mchars[9] = 'x';
  else
#endif
    mchars[9] = '-';
  stralloc_catb(out, mchars, sizeof(mchars));
}

int
list_dir_internal(stralloc* dir, char type) {
  size_t l;
  struct dir_s d;
  stralloc pre;
  int dtype;
  int is_dir, is_symlink;
  size_t len;
#if !WINDOWS_NATIVE
  struct stat st;
  static dev_t root_dev;
#endif
  char *name, *s;
  (void)type;
  while(dir->len > 1 && IS_DIRSEP(dir->s[dir->len - 1])) dir->len--;
  stralloc_nul(dir);
#if !WINDOWS_NATIVE
  if(root_dev == 0) {
    if(stat(dir->s, &st) != -1) {
      root_dev = st.st_dev;
    }
  }
#endif
  if(dir_open(&d, dir->s) != 0) {
    buffer_puts(buffer_2, "ERROR: Opening directory ");
    buffer_putsa(buffer_2, dir);
    buffer_puts(buffer_2, " failed!\n");
    buffer_flush(buffer_2);
    goto end;
  }
  if(dir->s[dir->len - 1] != DIRSEP_C) stralloc_cats(dir, DIRSEP_S);
  l = dir->len;
  while((name = dir_read(&d))) {
    unsigned int mode = 0, nlink = 0, uid = 0, gid = 0;
    uint64 size = 0, mtime = 0;
    dir->len = l;
    if(str_equal(name, "") || str_equal(name, ".") || str_equal(name, "..")) {
      continue;
    }
    stralloc_readyplus(dir, str_len(name) + 1);
    str_copy(dir->s + dir->len, name);
    dir->len += str_len(name);
#if !WINDOWS_NATIVE
    if(lstat(dir->s, &st) != -1) {
      if(root_dev && st.st_dev) {
        if(st.st_dev != root_dev) {
          continue;
        }
      }
      is_symlink = !!S_ISLNK(mode);
    } else
#endif
      is_symlink = 0;
    dtype = dir_type(&d);
#if !WINDOWS_NATIVE
    if(S_ISLNK(st.st_mode)) {
      stat(dir->s, &st);
    }
    mode = st.st_mode;
#endif
    if(dtype) {
      is_dir = !!(dtype & D_DIRECTORY);
    } else {
#if WINDOWS_NATIVE
      is_dir = 0;
#else
      is_dir = !!S_ISDIR(mode);
#endif
    }
    if(dtype & D_SYMLINK) is_symlink = 1;
#if !WINDOWS_NATIVE
    nlink = st.st_nlink;
    uid = st.st_uid;
    gid = st.st_gid;
    size = st.st_size;
    mtime = st.st_mtime;
#else
    mode = (is_dir ? S_IFDIR : (is_symlink ? S_IFLNK : S_IFREG));
#if USE_READDIR
    if(!is_dir) {
      size = get_file_size(s); /* dir_INTERNAL(&d)->dir_entry->d_name); */
      mtime = get_file_time(s);
    } else {
      mtime = 0;
      size = 0;
    }
#else
    size =
      ((uint64)(dir_INTERNAL(&d)->dir_finddata.nFileSizeHigh) << 32) + dir_INTERNAL(&d)->dir_finddata.nFileSizeLow;
    mtime = filetime_to_unix(&dir_INTERNAL(&d)->dir_finddata.ftLastWriteTime);
#endif
#endif
    if(opt_list) {
      stralloc_init(&pre);
      /* Mode string */
      mode_str(&pre, mode);
      stralloc_catb(&pre, " ", 1);
      /* num links */
      make_num(&pre, nlink, 3);
      stralloc_catb(&pre, " ", 1);
      /* uid */
      make_num(&pre, uid, 0);
      stralloc_catb(&pre, " ", 1);
      /* gid */
      make_num(&pre, gid, 0);
      stralloc_catb(&pre, " ", 1);
      /* size */
      make_num(&pre, size, 6);
      stralloc_catb(&pre, " ", 1);
      /* time */
      make_num(&pre, mtime, 0);
      /*     make_time(&pre, mtime, 10); */
      stralloc_catb(&pre, " ", 1);
    }
    /* fprintf(stderr, "%d %08x\n", is_dir, dir_ATTRS(&d)); */
    if(is_dir) stralloc_catc(dir, opt_separator);
    if(dir->len > MAXIMUM_PATH_LENGTH) {
      buffer_puts(buffer_2, "ERROR: Directory ");
      buffer_putsa(buffer_2, dir);
      buffer_puts(buffer_2, " longer than MAXIMUM_PATH_LENGTH (" STRINGIFY(MAXIMUM_PATH_LENGTH) ")!\n");
      /*buffer_putulong(buffer_2, MAXIMUM_PATH_LENGTH);
      buffer_puts(buffer_2, ")!\n");*/
      buffer_flush(buffer_2);
      goto end;
    }
    s = dir->s;
    len = dir->len;
    if(len >= 2 && s[0] == '.' && IS_DIRSEP(s[1])) {
      len -= 2;
      s += 2;
    }
    if(opt_list) buffer_putsa(buffer_1, &pre);
    if(opt_relative_to) {
      size_t sz = str_len(opt_relative_to);
      if(str_diffn(s, opt_relative_to, sz) == 0) {
        s += sz;
        len -= sz;
        while(*s == '\\' || *s == '/') {
          s++;
          len--;
        }
      }
    }
    buffer_put(buffer_1, s, len);
    buffer_put(buffer_1, "\n", 1);
    buffer_flush(buffer_1);
    if(is_dir && !is_symlink) {
      dir->len--;
      list_dir_internal(dir, 0);
    }
  }
end:
  dir_close(&d);
  return 0;
}

static ssize_t
io_err_check(ssize_t ret) {
  if(ret == -1) {
    buffer_putm_3(buffer_2, "ERROR: ", strerror(errno), "\n");
    buffer_flush(buffer_2);
    exit(errno);
    /* return -1; */
  }
  return ret;
}

static ssize_t
write_err_check(int fd, const void* buf, size_t len) {
  int ret = write(fd, buf, len);
  if(ret == -1) {
    buffer_putm_3(buffer_2, "ERROR: ", strerror(errno), "\n");
    buffer_flush(buffer_2);
    exit(errno);
    /* return -1; */
  }
  return ret;
}

void
usage(char* argv0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(argv0),
                       " [-o output] [infile or stdin]\n\n",
                       "  -1 ... -9           compression level; default is 3\n",
                       "\n",
                       "Options\n",
                       "  -h, --help                show this help\n",
                       "  -l, --list                long list\n",
                       "  -n, --numeric             numeric user/group\n",
                       "  -r, --relative            relative path\n",
                       "  -o, --output     FILE     write output to FILE\n",
                       "  -x, --exclude    PATTERN  exclude entries matching PATTERN\n",
                       "  -t, --time-style FORMAT   format time according to FORMAT\n", 0);
  buffer_putnlflush(buffer_1);
}

int
main(int argc, char* argv[]) {
  stralloc dir = {0, 0, 0};
  int c;
  int digit_optind = 0;
  const char* rel_to = 0;
  int index = 0;
  struct longopt opts[] = {
    {"help", 0, 0, 'h'},
    {"list", 0, &opt_list, 'l'},
    {"numeric", 0, &opt_numeric, 'n'},
    {"relative", 0, &opt_relative, 'r'},
    {"output", 1, 0, 'o'},
    {"exclude", 1, 0, 'x'},
    {"time-style", 1, 0, 't'},
#if WINDOWS
    {"separator", 1, 0, 's'},
#endif
    {0}
  };

#if WINDOWS && defined(O_BINARY)
  setmode(STDOUT_FILENO, O_BINARY);
#endif

  for(;;) {
    c = getopt_long(argc, argv, "hlnro:x:t:", opts, &index);
    if(c == -1) break;
    if(c == 0) continue;

    switch(c) {
    case 'h': usage(argv[0]); return 0;
    case 'x': {
      char* s = optarg;
      array_catb(&exclude_masks, (void*)&s, sizeof(char*));
      break;
    }
    case 'o': {
      buffer_1->fd = io_err_check(open_trunc(optarg));
      break;
    }
    case 't': {
      opt_timestyle = optarg;
      break;
    }
    case 's': {
      opt_separator = optarg[0];
      break;
    }
    case 'l':
    case 'n':
    case 'r':
      break;
    default: usage(argv[0]); return 1;
    }
  }
  /*
    while(optind < argc) {
      if(!str_diff(argv[optind], "-l") || !str_diff(argv[optind], "--list")) {
        opt_list = 1;
      } else if(!str_diff(argv[optind], "-n") || !str_diff(argv[optind], "--numeric")) {
        opt_numeric = 1;
      } else if(!str_diff(argv[optind], "-r") || !str_diff(argv[optind], "--relative")) {
        relative = 1;
      } else if(!str_diff(argv[optind], "-o") || !str_diff(argv[optind], "--output")) {
        buffer_1->fd = io_err_check(open_trunc(argv[optind + 1]));
        ++optind;
      } else if(!str_diff(argv[optind], "--relative")) {
        relative = 1;
      } else if(!str_diff(argv[optind], "-t") || !str_diff(argv[optind], "--time-style")) {
        optind++;
        opt_timestyle = argv[optind];
      } else {
        break;
      }
      optind++;
    }
    */
  array_catb(&exclude_masks, "\0\0\0\0\0\0\0\0", sizeof(char**));
  print_strarray(buffer_2, &exclude_masks);
  if(optind < argc) {
    while(optind < argc) {
      if(opt_relative) opt_relative_to = argv[optind];
      stralloc_copys(&dir, argv[optind]);
      list_dir_internal(&dir, 0);
      optind++;
    }
  } else {
    stralloc_copys(&dir, ".");
    list_dir_internal(&dir, 0);
  }
  return 0;
}
