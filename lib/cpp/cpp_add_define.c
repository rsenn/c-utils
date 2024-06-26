#include "../cpp_internal.h"
#include "../memstream.h"

int
cpp_add_define(cpp* pp, const char* mdecl) {
  int ret;
  size_t n;
  cpp_file tmp = {0};

  tmp.f = memstream_open(&tmp.buf, &tmp.len);
  buffer_puts(tmp.f, mdecl);
  buffer_putnlflush(tmp.f);

  if((n = byte_chr(tmp.buf, tmp.len, '=')) < tmp.len)
    tmp.buf[n] = ' ';

  // fprintf(tmp.f, "%s\n", mdecl);
  tmp.f = memstream_reopen(tmp.f, &tmp.buf, &tmp.len);

  tokenizer_from_file(&tmp.t, tmp.f);
  ret = cpp_macro_parse(pp, &tmp.t);

  free_file_container(&tmp);
  return ret;
}
