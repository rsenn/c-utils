#include "../windoze.h"
#include "../typedefs.h"
#include "../byte.h"

#if WINDOWS
#include "../ioctlcmd.h"
#include "../utf8.h"
#include "../stralloc.h"
#include <stdio.h>
#include <windows.h>
#include <winioctl.h>

#ifndef FILE_ATTRIBUTE_REPARSE_POINT
#define FILE_ATTRIBUTE_REPARSE_POINT 0x400
#endif
#ifndef FILE_FLAG_OPEN_REPARSE_POINT
#define FILE_FLAG_OPEN_REPARSE_POINT 0x200000
#endif

#ifndef Newx
#define Newx(v, n, t) v = (t*)malloc((n));
#endif

static BOOL
get_reparse_data(const char* LinkPath, REPARSE_DATA_BUFFER* rdb) {
  HANDLE hFile;
  DWORD returnedLength;

  int attr = GetFileAttributes(LinkPath);

  if(!(attr & FILE_ATTRIBUTE_REPARSE_POINT)) {
    return FALSE;
  }

  hFile =
      CreateFile(LinkPath,
                 0,
                 0,
                 0,
                 OPEN_EXISTING,
                 FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS,
                 0);

  if(hFile == INVALID_HANDLE_VALUE) {
    return FALSE;
  }

  /* Get the link */
  if(!DeviceIoControl(hFile,
                      FSCTL_GET_REPARSE_POINT,
                      0,
                      0,
                      &rdb->u,
                      1024,
                      &returnedLength,
                      0)) {

    CloseHandle(hFile);
    return FALSE;
  }

  CloseHandle(hFile);

  if(rdb->ReparseTag != IO_REPARSE_TAG_MOUNT_POINT &&
     rdb->ReparseTag != IO_REPARSE_TAG_SYMLINK) {
    return FALSE;
  }

  return TRUE;
}

#if WINDOWS_NATIVE
ssize_t
readlink(const char* LinkPath, char* buf, size_t maxlen) {
  REPARSE_DATA_BUFFER rdb;
  wchar_t* wbuf = 0;
  unsigned int u8_len, len, wlen;

  if(!get_reparse_data(LinkPath, &rdb)) {
    return -1;
  }

  switch(rdb.ReparseTag) {
    case IO_REPARSE_TAG_MOUNT_POINT: { /* Junction */
      wbuf = rdb.u.MountPointReparseBuffer.PathBuffer +
             rdb.u.MountPointReparseBuffer.SubstituteNameOffset /
                 sizeof(wchar_t);
      wlen = rdb.u.MountPointReparseBuffer.SubstituteNameLength /
             sizeof(WCHAR);
      break;
    }
    case IO_REPARSE_TAG_SYMLINK: { /* Symlink */
      wbuf = rdb.u.SymbolicLinkReparseBuffer.PathBuffer +
             rdb.u.SymbolicLinkReparseBuffer.SubstituteNameOffset /
                 sizeof(WCHAR);
      wlen = rdb.u.SymbolicLinkReparseBuffer.SubstituteNameLength /
             sizeof(WCHAR);
      break;
    }
  }

  if(!wbuf)
    return 0;

  for(len = 0; len < wlen; ++len) {
    u8_len += wc_u8len(wbuf[len]);

    if(u8_len >= maxlen)
      break;
  }
  if(u8_len > maxlen) {
    len--;
  }

  u8_len = wcs_to_u8s(buf, wbuf, len);
  if(u8_len >= maxlen)
    u8_len = maxlen - 1;

  buf[u8_len] = '\0';
  return u8_len;
}
#endif

static DWORD
reparse_tag(const char* LinkPath) {
  REPARSE_DATA_BUFFER rdb;

  if(!get_reparse_data(LinkPath, &rdb)) {
    return 0;
  }

  return rdb.ReparseTag;
}

int
is_symlink(const char* LinkPath) {
  return reparse_tag(LinkPath) == IO_REPARSE_TAG_SYMLINK;
}

char
is_junction(const char* LinkPath) {
  return reparse_tag(LinkPath) == IO_REPARSE_TAG_MOUNT_POINT;
}

#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int
is_symlink(const char* path) {
  struct stat st;
  if(lstat(path, &st) == 0) {
    if(S_ISLNK(st.st_mode))
      return 1;
  }
  return 0;
}

int
is_junction(const char* path) {
  struct stat st;
  int fd, ret = 0;
  if(stat(path, &st) == 0) {

    if((fd = open("/proc/mounts", O_RDONLY)) != -1) {
      char *p, *e;

      if((p = mmap(0, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0))) {
        size_t len;
        e = p + st.st_size;

        for(; p < e; p += len) {
          len = byte_chr(p, e - p, '\n');
        }
      }

      close(fd);
    }
  }
  return ret;
}

#endif /* WINDOWS */
