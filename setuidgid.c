#include "lib/errmsg.h"
#include "lib/scan.h"
#include "lib/open.h"
#include "lib/mmap.h"
#include "lib/str.h"
#include "lib/byte.h"
#include "lib/path.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

char*
search_path(const char* path, const char* what, stralloc* out) {
  const char* x;
  char* ret = 0;
  size_t pos;
  stralloc stra;

  stralloc_init(&stra);

  for(x = path; *x; x += pos) {
    pos = str_chr(x, ':');

    stralloc_copyb(&stra, x, pos);
    stralloc_catc(&stra, '/');
    stralloc_cats(&stra, what);
    stralloc_nul(&stra);

    if(path_exists(stra.s)) {
      stralloc_copy(out, &stra);
      stralloc_nul(out);
      ret = out->s;
      break;
    }

    pos++;
  }
  stralloc_free(&stra);
  return ret;
}

int
get_account(const char* name, int* uid, int* gid) {
  size_t p = 0, n, i, len;
  char *x;
  if((x = (char*)mmap_read("/etc/passwd", &n)) == NULL || n == 0)
    return 0;
 // end = x + n;
  len = str_len(name);
  while(p + len + 1 < n) {
    if(x[p + len] == ':' && !byte_diff(&x[p], len, name)) {
      p += len + 1;
      p += 1 + byte_chr(&x[p], n - p, ':');
      if(p < n) {
        i = scan_int(&x[p], uid);
        p += i;
        i = !!i;
        if(p < n && x[p] == ':')
          i += !!scan_int(&x[p + 1], gid);
        return i;
      }
    }
    p += 1 + byte_chr(&x[p], n - p, '\n');
  }
  mmap_unmap(x, n);
  return 0;
}

int
main(int argc, char* argv[], char* envp[]) {
  const char* account;
  char* prog = argv[0];
  stralloc full_path;
  int uid = -1, gid = -1;
  int res;
  account = *++argv;

  if(!account || !*++argv) {
    errmsg_warn("setuidgid: usage: setuidgid account child", 0);
    return 100;
  }

  if(!scan_int(account, &uid)) {
    if(!(res = get_account(account, &uid, &gid))) {
      errmsg_warn("setuidgid: no such account: ", account, 0);
      return 111;
    }
  }

  if(gid != -1)
    setgid(gid);
  if(uid != -1)
    setuid(uid);

  if(prog[str_chr(prog, '/')] == '\0') {
    stralloc_init(&full_path);
    prog = search_path(getenv("PATH"), prog, &full_path);
  }

  execve(prog, argv, envp);
  errmsg_warn("setuidgid: unable to run ", argv[0], ": ", 0);
  return 111;
}
