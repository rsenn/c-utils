#include "../ucs.c"
#include "../fmt.c"

size_t
fmt_latin1_utf8(char* out, char ch) {
  wchar_t wc = ucs_from_latin1_char(ch);
  return fmt_utf8(out, wc);
}
