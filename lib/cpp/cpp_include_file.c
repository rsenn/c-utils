#include "../cpp_internal.h"
#include "../open.h"
#include "../errmsg.h"

int
cpp_include_file(cpp_t* cpp, tokenizer* t, buffer* out) {
  static const char* const inc_chars[] = {"\"", "<", 0};
  static const char* const inc_chars_end[] = {"\"", ">", 0};
  struct token_s tok;
  int inc1sep, ret;
  tokenizer_set_flags(t, 0); // disable string tokenization

  inc1sep = expect(t, TT_SEP, inc_chars, &tok);
  if(inc1sep == -1) {
    error("expected one of [\"<]", t, &tok);
    return 0;
  }
  ret = tokenizer_read_until(t, inc_chars_end[inc1sep], 1);
  if(!ret) {
    error("error parsing filename", t, &tok);
    return 0;
  }
  {
    LIST_ITER_T i;
    fd_t fd = -1;
    buffer in;
    int ret;
    const char* fn;
    // TODO: different path lookup depending on whether " or <
    LIST_FOREACH(cpp->includedirs, i) {
      char buf[512];
      size_t pos = str_copy(buf, LIST_DATA(cpp->includedirs, i));
      buf[pos++] = '/';
      str_copyn(&buf[pos], t->buf, sizeof(buf) - pos);

      if((fd = open_read(buf)) != -1)

        break;
    }
    if(fd == -1) {
      errmsg_warnsys(t->buf, ": open", 0);

      return 0;
    }
    fn = str_dup(t->buf);
    assert(tokenizer_next(t, &tok) && is_char(&tok, inc_chars_end[inc1sep][0]));

    tokenizer_set_flags(t, TF_PARSE_STRINGS);

    buffer_read_fd(&in, fd);
    ret = cpp_parse_file(cpp, &in, fn, out);
    buffer_close(&in);
    buffer_free(&in);

    return ret;
  }
}
