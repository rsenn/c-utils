#include "../terminal_internal.h"

#if !WINDOWS_NATIVE
/* terminal_seq writes a CSI sequence, an empty parameter list writes none. */
void
terminal_seq(struct terminal* t, char prefix, const int* p, size_t n, char final) {
  size_t i;

  buffer_putc(t->out, 27);
  buffer_putc(t->out, '[');
  if(prefix)
    buffer_putc(t->out, prefix);
  for(i = 0; i < n; ++i) {
    if(i)
      buffer_putc(t->out, ';');
    buffer_putulong(t->out, (unsigned long)p[i]);
  }
  buffer_putc(t->out, final);
}
#endif
