#include "buffer.h"
#include "strlist.h"
#include <stdlib.h>
#include <getopt.h>

void
pathtool(const char* arg) {
  stralloc sa;
  strlist path;
  strlist_init(&path);
  strlist_push(&path, "");
  strlist_push_tokens(&path, arg, "/");

  stralloc_init(&sa);
  strlist_join(&path, &sa, '/');

  buffer_putsa(buffer_1, &sa);
  buffer_putnlflush(buffer_1);
}
int
main(int argc, char* argv[]) {
  struct option opts[] = {
    { "relative-to", 1, NULL },
  };

  while(optind < argc) {
    pathtool(argv[optind++]);
  }

  return 0;
}
