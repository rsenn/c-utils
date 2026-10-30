#include "../ini.h"
#include "../fmt.h"

void
ini_set_long(ini_section_t* ini, const char* key, long value) {
  char buf[FMT_LONG + 1];
  size_t n;

  n = fmt_long(buf, value);
  buf[n] = '\0';
  ini_set(ini, key, buf);
}
