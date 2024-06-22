#include <assert.h>

#define MAP_USE_HMAP 1

#include "cpp.h"
#include "map.h"
#include "pplist.h"
#include "buffer.h"
#include "memstream.h"
#include "tokenizer.h"
#include "alloc.h"
#include "str.h"
#include "scan.h"

#define MACRO_FLAG_OBJECTLIKE (1U << 31)
#define MACRO_FLAG_VARIADIC (1U << 30)
#define MACRO_ARGCOUNT_MASK (~(0 | MACRO_FLAG_OBJECTLIKE | MACRO_FLAG_VARIADIC))

#define OBJECTLIKE(M) (M->num_args & MACRO_FLAG_OBJECTLIKE)
#define FUNCTIONLIKE(M) (!(OBJECTLIKE(M)))
#define MACRO_ARGCOUNT(M) (M->num_args & MACRO_ARGCOUNT_MASK)
#define MACRO_VARIADIC(M) (M->num_args & MACRO_FLAG_VARIADIC)

#define MAX_RECURSION 32

#define x_tokenizer_next(T, TOK) x_tokenizer_next_of(T, TOK, 1)

struct cpp_s {
  LIST_T includedirs;
  MAP_T macros;
  const char* last_file;
  int last_line;
  tokenizer* tchain[MAX_RECURSION];
};

static int bp(int);
static int charlit_to_int(const char*);
static int eat_whitespace(tokenizer*, token* token, int* count);
static int emit_error_or_warning(tokenizer*, int is_error);
static void error(const char*, tokenizer* t, token* curr);
static void error_or_warning(const char*, const char* type, tokenizer* t, token* curr);
static int expect(tokenizer*, enum tokentype tt, const char* const values[], token* token);
static int expr(tokenizer*, int rbp, int* err);
static void free_file_container(cpp_file*);
static void free_visited(char* visited[]);
static int is_char(token*, int ch);
static int is_whitespace_token(token*);
static int led(tokenizer*, int left, token* tok, int* err);
static int nud(tokenizer*, token* tok, int* err);
static int skip_next_and_ws(tokenizer*, token* tok);
static int tokenizer_peek_next_non_ws(tokenizer*, token* tok);
static int token_needs_string(token*);
static void warning(const char*, tokenizer* t, token* curr);
static int was_visited(const char*, char* visited[], unsigned rec_level);
static int x_tokenizer_next_of(tokenizer*, token* tok, int fail_unk);

static inline void
free_file_container(cpp_file* fc) {
  buffer_free(fc->f);
  alloc_free(fc->buf);
}

ssize_t buffer_dummyreadbuf(fd_t, void*, size_t, void*);

static inline buffer*
buffer_reopen(buffer* f, char** buf, size_t* size) {
  buffer* nb;
  buffer_flush(f);

  if((nb = alloc(sizeof(buffer)))) {
    buffer_init_free(nb, &buffer_dummyreadbuf, -1, *buf, *size);
  }

  alloc_free(f);

  return nb;

  /*buffer_close(f);

  char* x = 0;
  size_t n;

  buffer_flush(f);

  if(*buf && (n = *size)) {
    if(!(x = str_ndup(*buf, n))) {
      return 0;
    }
  }

  buffer_close(f);
  buffer_init_free(f, (void*)f->op, -1, x, n);
  return f;*/
}

static inline void
tokenizer_from_file(tokenizer* t, buffer* f) {
  tokenizer_init(t, f, TF_PARSE_STRINGS);
  tokenizer_set_filename(t, "<macro>");
  // tokenizer_rewind(t);
}

static inline int
token_needs_string(token* tok) {
  switch(tok->type) {
    case TT_IDENTIFIER:
    case TT_WIDECHAR_LIT:
    case TT_WIDESTRING_LIT:
    case TT_SQSTRING_LIT:
    case TT_DQSTRING_LIT:
    case TT_ELLIPSIS:
    case TT_HEX_INT_LIT:
    case TT_OCT_INT_LIT:
    case TT_DEC_INT_LIT:
    case TT_FLOAT_LIT:
    case TT_UNKNOWN: return 1;
    default: return 0;
  }
}

static inline void
emit_token(buffer* out, token* tok, const char* strbuf) {
  if(tok->type == TT_SEP) {
    buffer_putc(out, tok->value);
  } else if(strbuf && token_needs_string(tok)) {
    buffer_puts(out, strbuf);
  } else {
    buffer_puts(buffer_2, "oops, dunno how to handle tt ");
    buffer_putlong(buffer_2, (int)tok->type);

    buffer_putm_internal(buffer_2, " (", strbuf, ")", NULL);
    buffer_putnlflush(buffer_2);
  }
}

static inline void
error_or_warning(const char* err, const char* type, tokenizer* t, token* curr) {
  int i;
  unsigned column = curr ? curr->column : t->column;
  unsigned line = curr ? curr->line : t->line;

  buffer_putm_internal(buffer_2, "<", t->filename, "> ", NULL);
  buffer_putulong(buffer_2, line);
  buffer_putc(buffer_2, ':');
  buffer_putulong(buffer_2, column);
  buffer_putm_internal(buffer_2, " ", type, ": '", err, "'\n", t->buf, "\n", NULL);

  /*  dprintf(2, "<%s> %u:%u %s: '%s'\n", t->filename, line, column, type, err);
    dprintf(2, "%s\n", t->buf);*/
  for(i = 0; i < str_len(t->buf); i++)
    buffer_puts(buffer_2, "^");

  buffer_putnlflush(buffer_2);
}

static inline void
error(const char* err, tokenizer* t, token* curr) {
  error_or_warning(err, "error", t, curr);
}

static inline void
warning(const char* err, tokenizer* t, token* curr) {
  error_or_warning(err, "warning", t, curr);
}

static inline void
flush_whitespace(buffer* out, int* ws_count) {
  while(*ws_count > 0) {
    buffer_puts(out, " ");
    --(*ws_count);
  }
}

static inline int
is_char(token* tok, int ch) {
  return tok->type == TT_SEP && tok->value == ch;
}

static inline int
is_whitespace_token(token* token) {
  return token->type == TT_SEP && (token->value == ' ' || token->value == '\t');
}

static inline size_t
macro_arglist_pos(cpp_macro* m, const char* iden) {
  size_t i, len = LIST_SIZE(m->argnames);

  for(i = 0; i < len; i++) {
    char* item = LIST_GET(m->argnames, i);
    if(!str_diff(item, iden))
      return i;
  }
  return (size_t)-1;
}

static inline int
x_tokenizer_next_of(tokenizer* t, token* tok, int fail_unk) {
  int ret = tokenizer_next(t, tok);
  if(tok->type == TT_OVERFLOW) {
    error("max token length of 4095 exceeded!", t, tok);
    return 0;
  } else if(fail_unk && ret == 0) {
    error("tokenizer encountered unknown token", t, tok);
    return 0;
  }
  return 1;
}

static inline int
mem_tokenizers_join(cpp_file* org, cpp_file* inj, cpp_file* result, int first, off_t lastpos) {
  size_t i;
  token tok;
  int ret, diff, cnt = 0, last = first;
  result->f = memstream_open(&result->buf, &result->len);
  tokenizer_rewind(&org->t);
  for(i = 0; i < first; ++i) {
    ret = tokenizer_next(&org->t, &tok);
    assert(ret && tok.type != TT_EOF);
    emit_token(result->f, &tok, org->t.buf);
  }
  while(1) {
    ret = tokenizer_next(&inj->t, &tok);
    if(!ret || tok.type == TT_EOF)
      break;
    emit_token(result->f, &tok, inj->t.buf);
    ++cnt;
  }
  while(tokenizer_ftello(&org->t) < lastpos) {
    ret = tokenizer_next(&org->t, &tok);
    last++;
  }

  diff = cnt - ((int)last - (int)first);

  while(1) {
    ret = tokenizer_next(&org->t, &tok);
    if(!ret || tok.type == TT_EOF)
      break;
    emit_token(result->f, &tok, org->t.buf);
  }

  result->f = buffer_reopen(result->f, &result->buf, &result->len);
  tokenizer_from_file(&result->t, result->f);
  return diff;
}

#define TT_LAND TT_CUSTOM + 0
#define TT_LOR TT_CUSTOM + 1
#define TT_LTE TT_CUSTOM + 2
#define TT_GTE TT_CUSTOM + 3
#define TT_SHL TT_CUSTOM + 4
#define TT_SHR TT_CUSTOM + 5
#define TT_EQ TT_CUSTOM + 6
#define TT_NEQ TT_CUSTOM + 7
#define TT_LT TT_CUSTOM + 8
#define TT_GT TT_CUSTOM + 9
#define TT_BAND TT_CUSTOM + 10
#define TT_BOR TT_CUSTOM + 11
#define TT_XOR TT_CUSTOM + 12
#define TT_NEG TT_CUSTOM + 13
#define TT_PLUS TT_CUSTOM + 14
#define TT_MINUS TT_CUSTOM + 15
#define TT_MUL TT_CUSTOM + 16
#define TT_DIV TT_CUSTOM + 17
#define TT_MOD TT_CUSTOM + 18
#define TT_LPAREN TT_CUSTOM + 19
#define TT_RPAREN TT_CUSTOM + 20
#define TT_LNOT TT_CUSTOM + 21

#define TTINT(X) (X - TT_CUSTOM)
#define TTENT(X, Y) [TTINT(X)] = Y

static inline int
bp(int tokentype) {
  static const int bplist[] = {
      0x20, 0x10,   0x400,  0x400,  0x800,  0x800,  0x200,  0x200,  0x400, 0x400, 0x100, 0x40,
      0x80, 0x4000, 0x1000, 0x1000, 0x2000, 0x2000, 0x2000, 0x8000, 0,     0x4000
      /*TTENT(TT_LOR, 1 << 4),
      TTENT(TT_LAND, 1 << 5),
      TTENT(TT_BOR, 1 << 6),
      TTENT(TT_XOR, 1 << 7),
      TTENT(TT_BAND, 1 << 8),
      TTENT(TT_EQ, 1 << 9),
      TTENT(TT_NEQ, 1 << 9),
      TTENT(TT_LTE, 1 << 10),
      TTENT(TT_GTE, 1 << 10),
      TTENT(TT_LT, 1 << 10),
      TTENT(TT_GT, 1 << 10),
      TTENT(TT_SHL, 1 << 11),
      TTENT(TT_SHR, 1 << 11),
      TTENT(TT_PLUS, 1 << 12),
      TTENT(TT_MINUS, 1 << 12),
      TTENT(TT_MUL, 1 << 13),
      TTENT(TT_DIV, 1 << 13),
      TTENT(TT_MOD, 1 << 13),
      TTENT(TT_NEG, 1 << 14),
      TTENT(TT_LNOT, 1 << 14),
      TTENT(TT_LPAREN, 1 << 15),
      //    TTENT(TT_RPAREN, 1 << 15),
      //    TTENT(TT_LPAREN, 0),
      TTENT(TT_RPAREN, 0)*/
  };
  if(TTINT(tokentype) < sizeof(bplist) / sizeof(bplist[0]))
    return bplist[TTINT(tokentype)];
  return 0;
}

/* skips until the next non-whitespace token (if the current one is one too)*/
static inline int
eat_whitespace(tokenizer* t, token* token, int* count) {
  int ret = 1;
  *count = 0;
  while(is_whitespace_token(token)) {
    ++(*count);
    ret = x_tokenizer_next(t, token);
    if(!ret)
      break;
  }
  return ret;
}

/* fetches the next token until it is non-whitespace */
static inline int
skip_next_and_ws(tokenizer* t, token* tok) {
  int ws_count, ret;

  if(!(ret = tokenizer_next(t, tok)))
    return ret;

  ret = eat_whitespace(t, tok, &ws_count);
  return ret;
}

static inline int
led(tokenizer* t, int left, token* tok, int* err) {
  int right;
  switch((unsigned)tok->type) {
    case TT_LAND:
    case TT_LOR:
      right = expr(t, bp(tok->type), err);
      if(tok->type == TT_LAND)
        return left && right;
      return left || right;
    case TT_LTE: return left <= expr(t, bp(tok->type), err);
    case TT_GTE: return left >= expr(t, bp(tok->type), err);
    case TT_SHL: return left << expr(t, bp(tok->type), err);
    case TT_SHR: return left >> expr(t, bp(tok->type), err);
    case TT_EQ: return left == expr(t, bp(tok->type), err);
    case TT_NEQ: return left != expr(t, bp(tok->type), err);
    case TT_LT: return left < expr(t, bp(tok->type), err);
    case TT_GT: return left > expr(t, bp(tok->type), err);
    case TT_BAND: return left & expr(t, bp(tok->type), err);
    case TT_BOR: return left | expr(t, bp(tok->type), err);
    case TT_XOR: return left ^ expr(t, bp(tok->type), err);
    case TT_PLUS: return left + expr(t, bp(tok->type), err);
    case TT_MINUS: return left - expr(t, bp(tok->type), err);
    case TT_MUL: return left * expr(t, bp(tok->type), err);
    case TT_DIV:
    case TT_MOD:
      right = expr(t, bp(tok->type), err);
      if(right == 0) {
        error("eval: div by zero", t, tok);
        *err = 1;
      } else if(tok->type == TT_DIV)
        return left / right;
      else if(tok->type == TT_MOD)
        return left % right;
      return 0;
    default:
      error("eval: unexpect token", t, tok);
      *err = 1;
      return 0;
  }
}

static inline int
expr(tokenizer* t, int rbp, int* err) {
  token tok;
  int left, ret = skip_next_and_ws(t, &tok);

  if(tok.type == TT_EOF)
    return 0;

  left = nud(t, &tok, err);

  while(1) {
    ret = tokenizer_peek_next_non_ws(t, &tok);
    if(bp(tok.type) <= rbp)
      break;
    ret = tokenizer_next(t, &tok);
    if(tok.type == TT_EOF)
      break;
    left = led(t, left, &tok, err);
  }

  (void)ret;
  return left;
}

static inline int
emit_error_or_warning(tokenizer* t, int is_error) {
  int ws_count, ret;
  token tmp;

  if(!(ret = tokenizer_skip_chars(t, " \t", &ws_count)))
    return ret;

  tmp.column = t->column;
  tmp.line = t->line;

  ret = tokenizer_read_until(t, "\n", 1);

  if(is_error) {
    error(t->buf, t, &tmp);
    return 0;
  }

  warning(t->buf, t, &tmp);
  return 1;
}

/* return index of matching item in values array, or -1 on error */
static inline int
expect(tokenizer* t, enum tokentype tt, const char* const values[], token* token) {
  int ret, i;

  do {
    ret = tokenizer_next(t, token);
    if(ret == 0 || token->type == TT_EOF)
      goto err;
  } while(is_whitespace_token(token));

  if(token->type != tt) {
  err:
    error("unexpected token", t, token);
    return -1;
  }

  for(i = 0; values[i]; i++)
    if(!str_diff(values[i], t->buf))
      return i;

  return -1;
}

static inline void
free_visited(char* visited[]) {
  size_t i;
  for(i = 0; i < MAX_RECURSION; i++)
    if(visited[i])
      alloc_free(visited[i]);
}

static inline int
charlit_to_int(const char* lit) {
  unsigned int ret = lit[1];

  if(lit[1] == '\\')
    switch(lit[2]) {
      case '0': ret = 0; break;
      case 'n': ret = 10; break;
      case 't': ret = 9; break;
      case 'r': ret = 13; break;
      case 'x':
        if(scan_xint(lit + 3, &ret))
          break;
      default: return lit[2];
    }
  return ret;
}

static inline int
nud(tokenizer* t, token* tok, int* err) {
  int ret;
  switch((unsigned)tok->type) {
    case TT_IDENTIFIER: ret = 0; break;
    case TT_WIDECHAR_LIT:
    case TT_SQSTRING_LIT: return charlit_to_int(t->buf);
    case TT_NEG: return ~expr(t, bp(tok->type), err);
    case TT_PLUS: return expr(t, bp(tok->type), err);
    case TT_MINUS: return -expr(t, bp(tok->type), err);
    case TT_LNOT: return !expr(t, bp(tok->type), err);
    case TT_LPAREN: {
      int inner = expr(t, 0, err);
      const char* values[] = {")", 0};
      if(0 != expect(t, TT_RPAREN, values, tok)) {
        error("missing ')'", t, tok);
        return 0;
      }
      return inner;
    }
    case TT_FLOAT_LIT:
      error("floating constant in preprocessor expression", t, tok);
      *err = 1;
      return 0;
    case TT_HEX_INT_LIT: scan_xint(t->buf, (unsigned int*)&ret); break;
    case TT_OCT_INT_LIT: scan_8int(t->buf, (unsigned int*)&ret); break;
    case TT_DEC_INT_LIT: scan_int(t->buf, &ret); break;

    case TT_RPAREN:

    default:
      error("unexpected token", t, tok);
      *err = 1;
      return 0;
  }
  return ret;
}

static inline int
tokenizer_peek_next_non_ws(tokenizer* t, token* tok) {
  int ret;
  while(1) {
    ret = tokenizer_peek_token(t, tok);
    if(is_whitespace_token(tok))
      x_tokenizer_next(t, tok);
    else
      break;
  }
  return ret;
}

static inline int
was_visited(const char* name, char* visited[], unsigned rec_level) {
  int x;
  for(x = rec_level; x >= 0; --x) {
    if(!str_diff(visited[x], name))
      return 1;
  }
  return 0;
}
