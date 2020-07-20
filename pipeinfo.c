#include "lib/uint32.h"
#include "lib/uint64.h"
#include "lib/windoze.h"
#include "lib/buffer.h"
#include "lib/errmsg.h"
#include "lib/dir.h"
#include "lib/path.h"
#include "lib/str.h"
#include "lib/stralloc.h"
#include "lib/scan.h"
#include "lib/mmap.h"
#include "lib/seek.h"
#include "lib/open.h"
#include "lib/byte.h"
#include "lib/getopt.h"

#include <ctype.h>
#include <sys/stat.h>

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

#ifdef __LCC__
#define stat _stat
#endif

static int verbose;

void
usage(char* av0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(av0),
                       " [OPTIONS] <FILE.list | TARGET LINK>\n",
                       "\n",
                       "Options:\n",
                       "\n",
                       "  -h, --help              Show this help\n",
                       "  -v, --verbose           Be verbose\n",
                       "\n",
                       NULL);
  buffer_flush(buffer_1);
}

void
print_number(const char* property, int64 num) {

  buffer_putm_internal(buffer_1, property, " = ", 0);
  buffer_putlonglong(buffer_1, num);
  buffer_putnlflush(buffer_1);
}

const char*
type(uint32 bits) {
  switch(bits & S_IFMT) {
    case S_IFSOCK: return "socket";
    case S_IFLNK: return "symlink";
    case S_IFREG: return "regular";
    case S_IFBLK: return "blkdev";
    case S_IFDIR: return "directory";
    case S_IFCHR: return "chardev";
    case S_IFIFO: return "fifo";
    default: return NULL;
  }
}
void
print_stat(const char* property, const struct stat* st) {
  const char* t = type(st->st_mode);
  buffer_putm_internal(buffer_1, property, ": ", 0);
  t ? buffer_puts(buffer_1, t) : buffer_put8long(buffer_1, st->st_mode & S_IFMT);

  buffer_putm_internal(buffer_1, " [mode=0", 0);
  buffer_put8long(buffer_1, st->st_mode & 07777);
  if(st->st_dev) {
    buffer_putm_internal(buffer_1, " dev=0x", 0);
    buffer_putxlong0(buffer_1, st->st_dev, 3);
  }
  if(st->st_rdev) {
    buffer_putm_internal(buffer_1, " rdev=0x", 0);
    buffer_putxlong0(buffer_1, st->st_rdev, 3);
  }
  if(st->st_ino) {
    buffer_putm_internal(buffer_1, " ino=0x", 0);
    buffer_putxlong0(buffer_1, st->st_ino, 3);
  }
  if(st->st_size) {
    buffer_putm_internal(buffer_1, " size=", 0);
    buffer_putulonglong(buffer_1, st->st_size);
  }
  if(st->st_blocks) {
    buffer_putm_internal(buffer_1, " blocks=", 0);
    buffer_putulonglong(buffer_1, st->st_blocks);
  }
  if(st->st_blksize) {
    buffer_putm_internal(buffer_1, " blksize=0x", 0);
    buffer_putxlonglong(buffer_1, st->st_blksize);
  }
  buffer_puts(buffer_1, "]");
  buffer_putnlflush(buffer_1);
}

void
print_stralloc(const char* property, const stralloc* sa) {
  buffer_putm_internal(buffer_1, property, " = ", 0);
  buffer_putsa(buffer_1, sa);
  buffer_putnlflush(buffer_1);
}

void
print_string(const char* property, const char* str) {
  buffer_putm_internal(buffer_1, property, " = ", str, 0);
  buffer_putnlflush(buffer_1);
}

void
proc_fd_root(int32 pid, stralloc* out) {
  stralloc_zero(out);
  stralloc_cats(out, "/proc/");
  stralloc_catulong(out, pid <= 0 ? getpid() : pid);
  stralloc_nul(out);
}

const char*
proc_fd_path(int32 pid, fd_t fd) {
  static stralloc sa;
  stralloc_zero(&sa);
  proc_fd_root(pid, &sa);
  stralloc_cats(&sa, "/fd/");
  if(fd >= 0)
    stralloc_catulong(&sa, fd);
  stralloc_nul(&sa);
  return sa.s;
}

void
read_proc() {
  dir_t procdir, fddir;
  uint32 pid, fd;
  const char *fdPath, targetPath;
  stralloc target, real, current;
  stralloc_init(&real);
  stralloc_init(&target);
  stralloc_init(&current);

  const char *fdStr, *s;
  if(dir_open(&procdir, "/proc"))
    return;

  while((s = dir_read(&procdir))) {
    if(!isdigit(s[0]))
      continue;

    if(scan_uint(s, &pid) > 0) {
      fdPath = proc_fd_path(pid, -1);

      stralloc_copys(&current, fdPath);
      stralloc_nul(&current);

      if(dir_open(&fddir, fdPath))
        continue;

      while((fdStr = dir_read(&fddir))) {
        if(!isdigit(fdStr[0]))
          continue;
        fd = -1;
        if(scan_uint(fdStr, &fd) > 0) {
          struct stat lst, st;
          byte_zero(&lst, sizeof(lst));
          byte_zero(&st, sizeof(st));
          fdPath = proc_fd_path(pid, fd);
          buffer_putm_internal(buffer_1, "fdPath: ", fdPath, " ", 0);

          if(lstat(fdPath, &lst) != -1)
            print_stat("\n  lst", &lst);

          if(stat(fdPath, &st) != -1)
            print_stat("\n  stat", &st);

          stralloc_zero(&target);

          if(S_ISLNK(lst.st_mode)) {
            path_readlink(fdPath, &target);
          }

          stralloc_zero(&real);
          path_realpath(fdPath, &real, 1, &current);

          if(stralloc_start(&real, &current))
            stralloc_remove(&real, 0, current.len);

          /*  if(!stralloc_contains(&real, "pipe"))
              continue;*/

          fd_t tmpfd = open_read(real.s);

          print_number("tmpfd", tmpfd);
          seek_end(tmpfd);

          uint64 n = seek_cur(tmpfd);
          if(n > 0)
            print_number("n", n);
          close(tmpfd);

          // print_number("pid", pid);
          if(S_ISLNK(lst.st_mode)) {
            if(!stralloc_equal(&real, &target))
              print_stralloc("real", &real);
            print_stralloc("target", &target);
          }

          buffer_putnlflush(buffer_1);
        }
      }
      buffer_putnlflush(buffer_1);
    }
  }
}

int
main(int argc, char* argv[]) {
  int index = 0, c;
  static const struct longopt opts[] = {{"help", 0, NULL, 'h'}, {"verbose", 0, 0, 'v'}, {0, 0, 0, 0}};

  errmsg_iam(argv[0]);

  for(;;) {
    c = getopt_long(argc, argv, "hv", opts, &index);
    if(c == -1)
      break;
    if(c == '\0')
      continue;

    switch(c) {
      case 'h': usage(argv[0]); return 0;
      case 'v': verbose = 1; break;
      default: {
        usage(argv[0]);
        return 1;
      }
    }
  }

  if(argc <= 1) {
    argv[optind] = "-";
    argc++;
  }

  read_proc();

  while(optind < argc) {
    const char* a = argv[optind++];
    fd_t fd = str_equal(a, "-") ? 0 : open_rw(a);

    buffer_putm_internal(buffer_1, "arg: '", a, "'", 0);
    buffer_putnlflush(buffer_1);
    struct stat st;

    if(fstat(fd, &st) != -1) {
      const char* fd_path = proc_fd_path(-1, fd);
      stralloc target;
      stralloc_init(&target);

      path_readlink(fd_path, &target);

      print_string("fd_path", fd_path);
      print_stralloc("target", &target);

      print_number("fd", fd);
      print_number("tar", fd);
      print_number("size", st.st_size);
      print_number("blksize", st.st_blksize);
      print_number("blocks", st.st_blocks);
    }
  }
}
