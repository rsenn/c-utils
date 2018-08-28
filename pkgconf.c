#include "lib/buffer.h"
#include "lib/getopt.h"

static int version = 0, cflags = 0, libs = 0;

int main(int argc, char* argv[]) {
     int c;
  int digit_optind = 0;
  const char* rel_to = NULL;
  int index = 0;
  struct option opts[] = {
    { "version", 0, NULL, 'v' },
    { "cflags",0, NULL, 'i' },
    { "libs", 0, NULL, 'l' },
  };

  for(;;) {
    c = getopt_long_only(argc, argv, "", opts, &index);
    if(c == -1)
      break;

    switch(c) {
    case 'v': version = 1; break;
    case 'i': cflags = 1; break;
    case 'l': libs = 1; break;
    default:
        break;
  }
}
