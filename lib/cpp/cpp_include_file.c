#include "../cpp_internal.h"
#include "../open.h"
#include "../errmsg.h"
#include "../path.h"

int
cpp_include_file(cpp* pp, tokenizer* t, buffer* out) {
  static const char* const inc_chars[] = {"\"", "<", 0};
  static const char* const inc_chars_end[] = {"\"", ">", 0};
  token tok;
  int inc1sep, ret;
  LIST_ITER_T i;
  fd_type fd = -1;
  buffer in;
  const char* fn;
  stralloc sa;
  stralloc_init(&sa);

  tokenizer_set_flags(t, 0); // disable string tokenization

  if((inc1sep = cpp_parse_expect(t, TT_SEP, inc_chars, &tok)) == -1) {
    error("expected one of [\"<]", t, &tok);
    return 0;
  }

  if(!(ret = tokenizer_read_until(t, inc_chars_end[inc1sep], 1))) {
    error("error parsing filename", t, &tok);
    return 0;
  }

  if(!path_is_absolute(t->buf)) {
    path_dirname(t->filename, &sa);
    path_appends(t->buf, &sa);
    path_collapse_sa(&sa);

    fd = open_read(sa.s);
  }

  if(fd == -1) {
    // TODO: different path lookup depending on whether " or <
    LIST_FOREACH(pp->includedirs, i) {
      stralloc_copys(&sa, LIST_DATA(pp->includedirs, i));

      path_appends(t->buf, &sa);
      path_collapse_sa(&sa);

      if((fd = open_read(sa.s)) != -1)
        break;
    }
  }

  if(fd == -1) {
    errmsg_warnsys("cpp_include_file: '", sa.s, "'", 0);
    return 0;
  }

#ifdef DEBUG_CPP
  buffer_puts(buffer_2, "including file '");
  buffer_putsa(buffer_2, &sa);
  buffer_putsflush(buffer_2, "'\n");
#endif

  stralloc_free(&sa);

  fn = str_dup(t->buf);
  assert(tokenizer_next(t, &tok) && token_is_char(&tok, inc_chars_end[inc1sep][0]));

  tokenizer_set_flags(t, TF_PARSE_STRINGS);

  buffer_read_fd(&in, fd);
  ret = cpp_parse_file(pp, &in, fn, out);
  buffer_close(&in);
  buffer_free(&in);

  return ret;
}
