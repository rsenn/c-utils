#include "../tokenizer_internal.h"

int
tokenizer_getc(tokenizer* t) {
  ssize_t ret;
  if(t->getc_buf.buffered) {
    t->getc_buf.buffered--;
    ret = t->getc_buf.buf[(t->getc_buf.cnt) % countof(t->getc_buf.buf)];
    ++t->getc_buf.cnt;
  } else {
    char ch;
    if((ret = buffer_getc(t->input, &ch)) > 0) {
      t->getc_buf.buf[t->getc_buf.cnt % countof(t->getc_buf.buf)] = ch;
      ++t->getc_buf.cnt;
      ret = ch;
    } else {
      if(ret == 0)
        ret = -2;
    }
  }
  return ret;
}