#include "lib/buffer.h"
#include "lib/dir.h"
#include "lib/getopt.h"
#include "lib/path.h"
#include "lib/strlist.h"
#include <stdbool.h>
#include <stdlib.h>

static struct {
  bool version;
  bool cflags;
  bool libs;
  bool list;
  strlist path;
  stralloc self;
} cmd;

void
pkg_list() {
  int i, n = strlist_count(&cmd.path);
  stralloc path;
  stralloc_init(&path);

  for(i = 0; i < n; ++i) {
    const char *entry;
    dir_t d;

    path = strlist_at_sa(&cmd.path, i);
    stralloc_0(&path);
    dir_open(&d, path.s);

    while((entry = dir_read(&d))) {
      stralloc_catm(&path, "/", entry);

      buffer_putsa(buffer_1, &path);
      buffer_putnlflush(buffer_1);
    }
  }
}

void
pkg_conf(const char* module) {}

int
main(int argc, char* argv[]) {
  int c;
  int digit_optind = 0;
  const char* rel_to = NULL;
  int index = 0;
  struct option opts[] = {
      {"modversion", 0, NULL, 'm'},
      {"cflags", 0, NULL, 'i'},
      {"libs", 0, NULL, 'l'},
      {"list-all", 0, NULL, 'a'},
  };

  for(;;) {
    c = getopt_long(argc, argv, "mila", opts, &index);
    if(c == -1) break;

    switch(c) {
      case 'm':
        cmd.version = true;
        break;
      case 'i':
        cmd.cflags = true;
        break;
      case 'l':
        cmd.libs = true;
        break;
      case 'a':
        cmd.list = true;
        break;
      default:
        break;
    }
  }

  path_readlink("/proc/self/exe", &cmd.self);

  strlist_froms(&cmd.path, getenv("PKG_CONFIG_PATH"), ':');

  if(strlist_count(&cmd.path) == 0) {
    stralloc prefix;
    stralloc_init(&prefix);
    stralloc_copy(&prefix, &cmd.self);
    size_t len = stralloc_finds(&prefix, "/bin");

    if(len == prefix.len) {
      stralloc_copys(&prefix, "/usr");
      len = prefix.len;
    }
    prefix.len = len;
    stralloc_cats(&prefix, "/lib/pkgconfig");
    strlist_push_sa(&cmd.path, &prefix);
    prefix.len = len;
    stralloc_cats(&prefix, "/share/pkgconfig");
    strlist_push_sa(&cmd.path, &prefix);
  }

  buffer_puts(buffer_2, "PKG_CONFIG_PATH is ");
  buffer_putsa(buffer_2, &cmd.path.sa);
  buffer_putnlflush(buffer_2);

  if(cmd.list) {
    pkg_list();

  } else {
    while(optind < argc) pkg_conf(argv[optind]++);
  }
}
