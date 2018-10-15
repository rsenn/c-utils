#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/errmsg.h"
#include "lib/path.h"
#include <ctype.h>
#include <sys/stat.h>

extern int symlink(const char*, const char*);

int
reduce(stralloc* sa) {
  ssize_t i, j;
  j = stralloc_finds(sa, ".so");

  for(i = sa->len - 1; i >= 0; --i) {
    if(!(isdigit(sa->s[i]) || sa->s[i] == '.')) break;
    if(i == j + 3 || sa->s[i] == '.') {
      sa->len = i;
      return 1;
    }
  }

  for(i = 0; i < sa->len; ++i) {
    if(sa->s[i] == '-' && isdigit(sa->s[i + 1])) {

      size_t k = i + 1;

      while(isdigit(sa->s[k])) ++k;

      if(!str_diffn(&sa->s[k], ".so", 3)) break;

      byte_copy(&sa->s[i], sa->len - j, &sa->s[j]);
      sa->len -= j - i;
      return 1;
    }
  }

  return 0;
}

int
mklink(char* target, char* link) {
  struct stat st;

  if(stat(target, &st) == -1) {
    errmsg_warnsys("stat failed", 0);
    return -1;
  }

#if !((defined(_WIN32) || defined(_WIN64)) && !(defined(__MSYS__) || defined(__CYGWIN__)))
  if(lstat(link, &st) != -1)
#endif
  {
    unlink(link);
  }

  return symlink(path_basename(target), link);
}

int
mklink_sa(stralloc* target, stralloc* link) {
  size_t i;

  stralloc_nul(target);
  stralloc_nul(link);

  if(stralloc_rchr(target, '/') == target->len && (i = stralloc_rchr(link, '/')) != link->len) {
    size_t len = i + 1;
    stralloc_insertb(target, link->s, 0, len);
  }

  buffer_putsa(buffer_2, link);
  buffer_puts(buffer_2, " -> ");
  buffer_putsa(buffer_2, target);
  buffer_putnlflush(buffer_2);

  stralloc_nul(target);
  return mklink(target->s, link->s);
}

int
sln(const char* path) {
  stralloc s, d;
  char* to;
  ssize_t i;
  stralloc_init(&s);
  stralloc_copys(&s, path);

  stralloc_init(&d);
  stralloc_copy(&d, &s);

  while(reduce(&d)) {

    buffer_puts(buffer_2, "'");
    buffer_putsa(buffer_2, &d);
    buffer_puts(buffer_2, "' -> '");
    buffer_putsa(buffer_2, &s);
    buffer_puts(buffer_2, "'\n");
    buffer_flush(buffer_2);

    stralloc_nul(&s);
    stralloc_nul(&d);

    if(mklink_sa(&s, &d) == -1) {
      errmsg_warnsys("symlink failed", NULL);
      exit(2); 
    }
    stralloc_copy(&s, &d);
  }

  return 0;
}
int
main(int argc, char* argv[]) {
  int i;
  for(i = 1; i < argc; ++i) {
    const char* a = argv[i];
    int i = str_rchr(a, '.');

    if(str_equal(&a[i], ".list")) {
      buffer in;
      if(!buffer_mmapread(&in, a)) {
        stralloc target, link;
        ssize_t ret;
        stralloc_init(&target);
        stralloc_init(&link);

        for(;;) {
          if((ret = buffer_get_new_token_sa(&in, &target, " \t\v", 2)) < 0) break;

          if(ret == 0 || target.s[0] == '\0') break;
          if(target.len > 0) --target.len;

          if((ret = buffer_get_new_token_sa(&in, &link, "\r\n", 2)) < 0) break;

          if(ret == 0 || link.s[0] == '\0') break;
          stralloc_chomp(&link);

          mklink_sa(&target, &link);

          if(!stralloc_endb(&link, ".so", 3)) {
            if(sln(link.s)) return 1;
          }
        }

        buffer_close(&in);
      }
    } else {
      if(sln(argv[i])) return 1;
    }
  }
}
