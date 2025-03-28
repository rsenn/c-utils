#ifndef __GNUC__

#include <string.h>
#include <assert.h>
#include "lib/tokenizer_internal.h"
#include "lib/cpp.h"
#include "lib/buffer.h"
#include "lib/open.h"
#include "lib/map.h"
#include "lib/errmsg.h"
#include "lib/alloc.h"
#include "lib/pplist.h"

static unsigned
string_hash(const char* s) {
  uint_fast32_t h = 0;

  while(*s) {
    h = 16 * h + *s++;
    h ^= h >> 24 & 0xf0;
  }

  return h & 0xfffffff;
}

static void
tokenizer_from_file(tokenizer* t, buffer* f) {
  tokenizer_init(t, f, TF_PARSE_STRINGS);
  tokenizer_set_filename(t, "<macro>");
  tokenizer_rewind(t);
}

static int
strptrcmp(const void* a, const void* b) {
  return str_diff(*(const char* const*)a, *(const char* const*)b);
}

static int
x_tokenizer_next_of(tokenizer* t, struct token* tok, int fail_unk) {
  int ret = tokenizer_next(t, tok);

  if(tok->type == TT_OVERFLOW) {
    error("max token length of 4095 exceeded!", t, tok);
    return 0;
  }

  if(fail_unk && ret == 0) {
    error("tokenizer encountered unknown token", t, tok);
    return 0;
  }

  return 1;
}

#define tokenizer_next(T, TOK) x_tokenizer_next_of(T, TOK, 0)
#define x_tokenizer_next(T, TOK) x_tokenizer_next_of(T, TOK, 1)

static int
is_whitespace_token(struct token* token) {
  return token->type == TT_SEP && (token->value == ' ' || token->value == '\t');
}

static int
is_char(struct token* tok, int ch) {
  return tok->type == TT_SEP && tok->value == ch;
}

static void
flush_whitespace(buffer* out, int* ws_count) {
  for(; *ws_count > 0; --(*ws_count))
    buffer_puts(out, " ");
}

static void
emit_token(buffer* out, struct token* tok, const char* strbuf) {
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

/* skips until the next non-whitespace token (if the current one is one
 * too)*/
static int
eat_whitespace(tokenizer* t, struct token* token, int* count) {
  *count = 0;
  int ret = 1;

  while(is_whitespace_token(token)) {
    ++(*count);
    ret = x_tokenizer_next(t, token);

    if(!ret)
      break;
  }
  return ret;
}

static int
emit_error_or_warning(tokenizer* t, int is_error) {
  int ws_count, ret;

  if((ret = tokenizer_skip_chars(t, " \t", &ws_count))) {
    struct token tmp = {.column = t->column, .line = t->line};

    ret = tokenizer_read_until(t, "\n", 1);

    if(is_error) {
      error(t->buf, t, &tmp);
      return 0;
    }

    warning(t->buf, t, &tmp);

    return 1;
  }

  return ret;
}

/* return index of matching item in values array, or -1 on error */
static int
expect(tokenizer* t, enum tokentype tt, const char* values[], struct token* token) {
  int ret;

  do {
    if((ret = tokenizer_next(t, token)) == 0 || token->type == TT_EOF)
      goto err;

  } while(is_whitespace_token(token));

  if(token->type != tt) {
  err:
    error("unexpected token", t, token);
    return -1;
  }

  for(int i = 0; values[i]; ++i)

    if(!str_diff(values[i], t->buf))
      return i;

  return -1;
}

static void
free_visited(char* visited[]) {
  for(size_t i = 0; i < MAX_RECURSION; i++)

    if(visited[i])
      alloc_free(visited[i]);
}

static int
led(tokenizer* t, int left, struct token* tok, int* err) {
  int right;

  switch((unsigned)tok->type) {
    case TT_LAND:
    case TT_LOR: {
      right = expr(t, bp(tok->type), err);

      if(tok->type == TT_LAND)
        return left && right;

      return left || right;
    }

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
    case TT_MOD: {
      if((right = expr(t, bp(tok->type), err)) == 0) {
        error("eval: div by zero", t, tok);
        *err = 1;
      } else if(tok->type == TT_DIV) {
        return left / right;
      } else if(tok->type == TT_MOD) {
        return left % right;
      }

      return 0;
    }

    default: {
      error("eval: unexpect token", t, tok);
      *err = 1;
      return 0;
    }
  }
}

static int
nud(tokenizer* t, struct token* tok, int* err) {
  switch((unsigned)tok->type) {
    case TT_IDENTIFIER: return 0;
    case TT_WIDECHAR_LIT:
    case TT_SQSTRING_LIT: return charlit_to_int(t->buf);
    case TT_HEX_INT_LIT:
    case TT_OCT_INT_LIT:
    case TT_DEC_INT_LIT: return strtol(t->buf, NULL, 0);
    case TT_NEG: return ~expr(t, bp(tok->type), err);
    case TT_PLUS: return expr(t, bp(tok->type), err);
    case TT_MINUS: return -expr(t, bp(tok->type), err);
    case TT_LNOT: return !expr(t, bp(tok->type), err);

    case TT_LPAREN: {
      int inner = expr(t, 0, err);

      if(0 != expect(t, TT_RPAREN, (const char*[]){")", 0}, tok)) {
        error("missing ')'", t, tok);
        return 0;
      }

      return inner;
    }

    case TT_FLOAT_LIT: {
      error("floating constant in preprocessor expression", t, tok);
      *err = 1;
      return 0;
    }

    case TT_RPAREN:
    default: {
      error("unexpected token", t, tok);
      *err = 1;
      return 0;
    }
  }
}

/* fetches the next token until it is non-whitespace */
static int
skip_next_and_ws(tokenizer* t, struct token* tok) {
  int ret, ws_count;

  if(!(ret = tokenizer_next(t, tok)))
    return ret;

  return eat_whitespace(t, tok, &ws_count);
}

static int
tokenizer_peek_next_non_ws(tokenizer* t, struct token* tok) {
  int ret;

  for(;;) {
    ret = tokenizer_peek_token(t, tok);

    if(is_whitespace_token(tok))
      x_tokenizer_next(t, tok);
    else
      break;
  }

  return ret;
}

static int
was_visited(const char* name, char* visited[], unsigned rec_level) {
  for(int x = rec_level; x >= 0; --x)

    if(!str_diff(visited[x], name))
      return 1;

  return 0;
}

static int
consume_nl_and_ws(tokenizer* t, struct token* tok, int expected) {
  if(!x_tokenizer_next(t, tok)) {
  err:
    error("unexpected", t, tok);
    return 0;
  }

  if(expected) {
    if(tok->type != TT_SEP || tok->value != expected)
      goto err;

    switch(expected) {
      case '\\': expected = '\n'; break;
      case '\n': expected = 0; break;
    }
  } else {
    if(is_whitespace_token(tok))
      ;
    else if(is_char(tok, '\\'))
      expected = '\n';
    else
      return 1;
  }

  return consume_nl_and_ws(t, tok, expected);
}

static size_t
macro_arglist_pos(struct macro* m, const char* iden) {
  size_t i, len = LIST_SIZE(m->argnames);

  for(i = 0; i < len; i++) {
    char* item = LIST_GET(m->argnames, i);

    if(!str_diff(item, iden))
      return i;
  }

  return (size_t)-1;
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

#define TTINT(X) X - TT_CUSTOM
#define TTENT(X, Y) [TTINT(X)] = Y

static int
bp(int tokentype) {
  static const int bplist[] = {
      TTENT(TT_LOR, 1 << 4),
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
      //		TTENT(TT_RPAREN, 1 << 15),
      //		TTENT(TT_LPAREN, 0),
      TTENT(TT_RPAREN, 0),
  };

  if(TTINT(tokentype) < sizeof(bplist) / sizeof(bplist[0]))
    return bplist[TTINT(tokentype)];

  return 0;
}

static int expr(tokenizer* t, int rbp, int* err);

static int
charlit_to_int(const char* lit) {
  if(lit[1] == '\\') {
    switch(lit[2]) {
      case '0': return 0;
      case 'n': return 10;
      case 't': return 9;
      case 'r': return 13;
      case 'x': return strtol(lit + 3, NULL, 16);
      default: return lit[2];
    }
  }

  return lit[1];
}

static int
expr(tokenizer* t, int rbp, int* err) {
  struct token tok;
  int left, ret = skip_next_and_ws(t, &tok);

  if(tok.type == TT_EOF)
    return 0;

  left = nud(t, &tok, err);

  for(;;) {
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

static int
do_eval(tokenizer* t, int* result) {
  int err = 0;

  tokenizer_register_custom_token(t, TT_LAND, "&&");
  tokenizer_register_custom_token(t, TT_LOR, "||");
  tokenizer_register_custom_token(t, TT_LTE, "<=");
  tokenizer_register_custom_token(t, TT_GTE, ">=");
  tokenizer_register_custom_token(t, TT_SHL, "<<");
  tokenizer_register_custom_token(t, TT_SHR, ">>");
  tokenizer_register_custom_token(t, TT_EQ, "==");
  tokenizer_register_custom_token(t, TT_NEQ, "!=");

  tokenizer_register_custom_token(t, TT_LT, "<");
  tokenizer_register_custom_token(t, TT_GT, ">");

  tokenizer_register_custom_token(t, TT_BAND, "&");
  tokenizer_register_custom_token(t, TT_BOR, "|");
  tokenizer_register_custom_token(t, TT_XOR, "^");
  tokenizer_register_custom_token(t, TT_NEG, "~");

  tokenizer_register_custom_token(t, TT_PLUS, "+");
  tokenizer_register_custom_token(t, TT_MINUS, "-");
  tokenizer_register_custom_token(t, TT_MUL, "*");
  tokenizer_register_custom_token(t, TT_DIV, "/");
  tokenizer_register_custom_token(t, TT_MOD, "%");

  tokenizer_register_custom_token(t, TT_LPAREN, "(");
  tokenizer_register_custom_token(t, TT_RPAREN, ")");
  tokenizer_register_custom_token(t, TT_LNOT, "!");

  *result = expr(t, 0, &err);
#ifdef DEBUG
  dprintf(2, "eval result: %d\n", *result);
#endif
  return !err;
}

#endif
