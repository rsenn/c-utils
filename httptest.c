#include "buffer.h"
#include "http.h"
#include "io.h"
#include "iopause.h"

int main(int argc, char* argv[]) {

  http h;

  http_init(&h, "www.google.ch", 80);
  int ret = http_get(&h, "/");

  buffer_puts(buffer_1, "http_get() = ");
  buffer_putlong(buffer_1, (long)ret);
  buffer_putnlflush(buffer_1);

  return 0;
}
