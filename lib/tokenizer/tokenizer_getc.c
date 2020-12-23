#include "../tokenizer_internal.h"
#include "../fmt.h"

int
tokenizer_getc(tokenizer* t) {
  ssize_t ret;
  if(t->chb.buffered) {
    t->chb.buffered--;
    ret = t->chb.buf[(t->chb.cnt) % countof(t->chb.buf)];
    ++t->chb.cnt;
  } else {
    char ch;
    if((ret = buffer_getc(t->input, &ch)) > 0) {
      t->chb.buf[t->chb.cnt % countof(t->chb.buf)] = ch;
      ++t->chb.cnt;
      ret = ch;
    } else {
      if(ret == 0)
        ret = -2;
    }
#ifdef DEBUG_TOKENIZER
    if(ret >= 0) {
      buffer_puts(buffer_2, "tokenizer_getc '");
      if(ret < 0x20)
        buffer_put_escaped(buffer_2, &ret, 1, &fmt_escapecharc);
      else
        buffer_putc(buffer_2, ret);
      if(ret >= 'A')
        buffer_putc(buffer_2, '\'');
      else {
        buffer_puts(buffer_2, "' 0x");
        buffer_putxlong0(buffer_2, ret, 2);
      }
      buffer_putnlflush(buffer_2);
    }
#endif
  }

  return ret;
}