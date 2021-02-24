#define _GNU_SOURCE
#include "lib/errmsg.h"
#include "lib/scan.h"
#include "lib/open.h"
#include "lib/mmap.h"
#include "lib/str.h"
#include "lib/byte.h"
#include "lib/path.h"
#include "lib/buffer.h"

#include <stdlib.h>

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
  char* x;
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
main(int argc, char* argv[] /*, char* envp[]*/) {
  const char* account;
  char *prog, *cmd, **args;
  stralloc full_path;
  int uid = -1, gid = -1;
  int res;

  prog = argv[0];

  account = argv[1];

  args = &argv[2];

  if(!account || *args == 0) {
    buffer_putm_internal(buffer_2, "account: ", account ? account : "NULL", " args[0]: ", args[0], NULL);
    buffer_putnlflush(buffer_2);
    errmsg_warn(prog,
                ": usage: setuidgid "
                "account child",
                0);
    return 100;
  }

  if(!scan_int(account, &uid)) {
    if(!(res = get_account(account, &uid, &gid))) {
      errmsg_warn(prog, ": no such account: ", account, 0);
      return 111;
    }
  }

  if(gid != -1)
    setgid(gid);
  if(uid != -1)
    setuid(uid);

  cmd = args[0];

  if(cmd[str_chr(cmd, '/')] == '\0') {
    stralloc_init(&full_path);
    cmd = search_path(getenv("PATH"), cmd, &full_path);
  }

  execve(cmd, args, environ);
  errmsg_warn(prog, ": unable to run ", cmd, ": ", 0);
  return 127;
}
