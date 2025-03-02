#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/str.h"
#include "../lib/buffer.h"
#include "../lib/textcode.h"

void
b64encode(const char* c, long len) {
  char* buf = malloc(len * 2 + 4);
  buffer_put(buffer_1, buf, fmt_base64(buf, c, len));

  if(isatty(1))
    buffer_putnlflush(buffer_1);
  else
    buffer_flush(buffer_1);
  free(buf);
}

int
main(int argc, char* argv[]) {
  int i;

  for(i = 1; i < argc; ++i) {
    b64encode(argv[i], str_len(argv[i]));
  }

  if(argc < 2) {
    char src[1024];
    int len;

    while((len = read(0, src, sizeof(src) - 1)) > 0) {
      if(len == -1)
        return (1);
      b64encode(src, len);
    }
  }
  return 0;
}
