#include "../cpp_internal.h"
#include "../memstream.h"

int
cpp_add_define(cpp* cpp, const char* mdecl) {
  int ret;
  cpp_file tmp = {0};
  tmp.f = memstream_open(&tmp.buf, &tmp.len);
  buffer_puts(tmp.f, mdecl);
  buffer_putnlflush(tmp.f);

  // fprintf(tmp.f, "%s\n", mdecl);
  tmp.f = buffer_reopen(tmp.f, &tmp.buf, &tmp.len);
  tokenizer_from_file(&tmp.t, tmp.f);
  ret = cpp_macro_parse(cpp, &tmp.t);
  free_file_container(&tmp);
  return ret;
}
