#include "lib/errmsg.h"
#include "lib/scan.h"
#include "lib/open.h"
#include "lib/mmap.h"

int
get_account(const char* name, int* uid, int* gid) {
  size_t p = 0, n, i, len;
  char *x, *end;
  if((x = mmap_read("/etc/passwd", &n)) == NULL || n == 0)
    return 0;
  end = x + n;
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
main(int argc, char* argv[]) {
  const char* account;
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

  execvp(argv[0], argv);
  errmsg_warn("setuidgid: unable to run ", argv[0], ": ", 0);
  return 111;
}