#include "lib/unix.h"
#include "lib/open.h"
#include "lib/uint64.h"
#include "lib/buffer.h"
#include "lib/errmsg.h"
#include "lib/str.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/tokenizer.h"

static tokenizer t;
static buffer in;
static size_t prev;

struct file_pos {
  const char* file;

  int64 offset;
  uint32 line, column;
};

static int
usage(char* a0) {
  buffer_putm_internal(buffer_2,
                       "example tokenizer\n"
                       "usage: ",
                       a0,
                       " [...options] file\n"
                       "if no filename or '-' is passed, stdin is used.",
                       0);
  buffer_putnlflush(buffer_2);
  return 1;
}

void
dump_charbuf(const tokenizer_charbuf* charbuf) {
  buffer_puts(buffer_2, "cnt=");
  buffer_putulong(buffer_2, charbuf->cnt);
  buffer_puts(buffer_2, ",buffered=");
  buffer_putulong(buffer_2, charbuf->buffered);
  buffer_putnlflush(buffer_2);
}
struct file_pos
filepos_new(const char* file, int64 offset) {
  struct file_pos ret = {0, 0, 0, 0};
  ret.file = file;
  ret.offset = offset;
  return ret;
}

struct file_pos
filepos_from_token(const token* tok) {
  struct file_pos ret;
  ret = filepos_new(0, -1);
  ret.line = tok->line;
  ret.column = tok->column;
  return ret;
}

struct file_pos
filepos_from_buffer(buffer* b, size_t byte_pos) {
  struct file_pos ret;
  ret = filepos_new(0, byte_pos);
  ret.line = byte_line(b->x, byte_pos) + 1;
  ret.column = byte_column(b->x, byte_pos) + 1;
  return ret;
}

void
filepos_dump(const struct file_pos* pos) {
  static size_t prev;

  if(pos->offset > -1) {
    buffer_puts(buffer_2, "@");
    buffer_putulonglong(buffer_2, pos->offset);

    if(pos->offset != prev) {
      buffer_puts(buffer_2, " 𝛥 ");
      buffer_putulonglong(buffer_2, pos->offset - prev);
    }
    prev = pos->offset;
    buffer_puts(buffer_2, " / ");
  }
  if(pos->file) {
    buffer_puts(buffer_2, pos->file);
    buffer_puts(buffer_2, ":");
  }
  buffer_putulong(buffer_2, pos->line);
  if(pos->column) {
    buffer_puts(buffer_2, ":");
    buffer_putulong(buffer_2, pos->column);
  }
  buffer_puts(buffer_2, ": ");
}

void
token_dump(const tokenizer* t, const token* tok, size_t len) {
  static size_t prev;
  buffer_puts(buffer_2, " ");
  buffer_putspad(buffer_2,
                 (((const char* const[]){"0",
                                         "TT_IDENTIFIER",
                                         "TT_SQSTRING_LIT",
                                         "TT_DQSTRING_LIT",
                                         "TT_ELLIPSIS",
                                         "TT_HEX_INT_LIT",
                                         "TT_OCT_INT_LIT",
                                         "TT_DEC_INT_LIT",
                                         "TT_FLOAT_LIT",
                                         "TT_SEP",
                                         "TT_UNKNOWN",
                                         "TT_OVERFLOW",
                                         "TT_WIDECHAR_LIT",
                                         "TT_WIDESTRING_LIT",
                                         "TT_EOF",
                                         "TT_CUSTOM "})[tok->type]),
                 17);

  if(tok->type >= TT_HEX_INT_LIT && tok->type <= TT_DEC_INT_LIT) {
    buffer_puts(buffer_2, " value=");
    buffer_putulong(buffer_2, tok->value);
  }
  if(len > 0) {
    buffer_puts(buffer_2, " len=");
    buffer_putulong(buffer_2, len);
    buffer_puts(buffer_2, " data = ");

    buffer_putfmt(buffer_2, &t->input->x[prev], len, &fmt_escapecharnonprintable);
  }

  buffer_puts(buffer_2, "▕⎹  ");
  prev = in.p;
}

int
main(int argc, char** argv) {
  int ret, c;
  char *tmp, *fn;
  fd_t fd;
  errmsg_iam(argv[0]);
  while((c = unix_getopt(argc, argv, "h")) != -1)
    switch(c) {
      default: return usage(argv[0]);
    }
  fn = 0;
  fd = STDIN_FILENO;
  if(argv[unix_optind] && str_diff(argv[unix_optind], "-")) {
    fn = argv[unix_optind];
    if((fd = open_read(fn)) == -1) {
      errmsg_warnsys("open_read", 0);
      return 1;
    }
  }
  buffer_read_fd(&in, fd);
  tokenizer_init(&t, &in, TF_PARSE_STRINGS);
  tokenizer_set_filename(&t, fn ? fn : argv[unix_optind]);
  {
    token tok;
    uint64 offset = 0;
    while((ret = tokenizer_next(&t, &tok)) > 0) {
      struct file_pos pos;
      if(tok.type != TT_SEP) {
        pos = filepos_from_token(&tok);
      } else {
        pos = filepos_from_buffer(&in, offset);
      }
      if(pos.file == 0)
        pos.file = t.filename;
      if(pos.offset == -1)
        pos.offset = offset;
      filepos_dump(&pos);
      if(tok.type != TT_SEP)
        token_dump(&t, &tok, t.chb.cnt - offset);
      dump_charbuf(&t.chb);
      buffer_dump(buffer_2, &in);
      offset = t.chb.cnt;
    }
  }
  if(in.fd != STDIN_FILENO)
    buffer_close(&in);
  return !ret;
}
