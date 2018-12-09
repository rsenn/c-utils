#include <string.h>
#include "../lib/buffer.h"
#include "../lib/textcode.h"
#include <unistd.h>

void
cescape(const char* c) {
  char* buf = alloca(str_len(c) * 5 + 1);
  buffer_put(buffer_1, buf, fmt_cescape(buf, c, str_len(c)));
  buffer_putnlflush(buffer_1);
}

int
main(int argc, char* argv[]) {
  int i;
  for(i = 1; i < argc; ++i) {
    cescape(argv[i]);
  }
  if(argc < 2) {
    char src[1024];
    int len = read(0, src, sizeof(src) - 1);
    if(len == -1)
      return (1);
    src[len] = 0;
    cescape(src);
  }
  return 0;
}
