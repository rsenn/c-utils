#include "lib/buffer.h"
#include "lib/getopt.h"
#include <stdbool.h>

static struct {
 bool version;
 bool cflags;
 bool libs;
 const char* path;
} cmd;

void
pkg_conf(const char* module) {



}

int
main(int argc, char* argv[]) {
  int c;
  int digit_optind = 0;
  const char* rel_to = NULL;
  int index = 0;
  struct option opts[] = {
      {"version", 0, NULL, 'v'},
      {"cflags", 0, NULL, 'i'},
      {"libs", 0, NULL, 'l'},
  };

  for(;;) {
    c = getopt_long_only(argc, argv, "", opts, &index);
    if(c == -1) break;

    switch(c) {
      case 'v':
        cmd.version = TRUE;
        break;
      case 'i':
        cmd.cflags = TRUE;
        break;
      case 'l':
        cmd.libs = TRUE;
        break;
      default:
        break;
    }
  }

  cmd.path = getenv("PKG_CONFIG_PATH");

  if(cmd.path == NULL || str_equal(cmd.path, "")) {
      cmd.path = "/usr/lib/pkgconfig:/usr/share/pkgconfig";
  }

  while(optind < argc )
      pkg_conf(argv[optind]++);

}
