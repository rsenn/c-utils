#include "../lib/buffer.h"
#include "../lib/textcode.h"
#include "../lib/str.h"
#include <string.h>
#include <unistd.h>
#include <alloca.h>

void
urlencode(const char* c) {
  char buf[str_len(c) * 3 + 1];
  buffer_put(buffer_1, buf, fmt_urlencoded(buf, c, str_len(c)));
  buffer_putnlflush(buffer_1);
}

int
main(int argc, char* argv[]) {
  int i;
  for(i = 1; i < argc; ++i) {
    urlencode(argv[i]);
  }
  if(argc < 2) {
    char src[1024];
    int len = read(0, src, sizeof(src) - 1);
    if(len == -1)
      return (1);
    src[len] = 0;
    urlencode(src);
  }
  return 0;
}
