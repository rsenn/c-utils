#include <assert.h>

#define MAP_USE_HMAP 1
//#define MAP_USE_HASHMAP 1

#include "cpp.h"
#include "map.h"
#include "pplist.h"
#include "buffer.h"
#include "memstream.h"
#include "alloc.h"
#include "tokenizer.h"
#include "alloc.h"
#include "str.h"
#include "scan.h"

#define MACRO_FLAG_OBJECTLIKE (1U << 31)
#define MACRO_FLAG_VARIADIC (1U << 30)
#define MACRO_ARGCOUNT_MASK \
  (~(0 | MACRO_FLAG_OBJECTLIKE | MACRO_FLAG_VARIADIC))

#define OBJECTLIKE(M) ((M)->num_args & MACRO_FLAG_OBJECTLIKE)
#define FUNCTIONLIKE(M) (!(OBJECTLIKE(M)))
#define MACRO_ARGCOUNT(M) (M->num_args & MACRO_ARGCOUNT_MASK)
#define MACRO_VARIADIC(M) (M->num_args & MACRO_FLAG_VARIADIC)

#define MAX_RECURSION 32

#define x_tokenizer_next(T, TOK) parse_next_of_x(T, TOK, 1)

struct cpp_s {
  LIST_T includedirs;
  MAP_T macros;
  const char* last_file;
  int last_line;
  tokenizer* tchain[MAX_RECURSION];
};

static int cpp_bp(int);
static int parse_next_of_x(tokenizer*, token* tok, int fail_unk);
static int charlit_to_int(const char*);
static void free_file_container(cpp_file*);
static void free_visited(char* visited[]);
static int was_visited(const char*, char* visited[], unsigned rec_level);

static inline void
free_file_container(cpp_file* fc) {
  /*if(fc->f) {
    buffer_close(fc->f);
  }

  if(fc->buf) {
    alloc_free(fc->buf);
    fc->buf = 0;
  }*/
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
error(const char* err, tokenizer* t, token* curr) {
  cpp_error_or_warning(err, "error", t, curr);
}

static inline void
warning(const char* err, tokenizer* t, token* curr) {
  cpp_error_or_warning(err, "warning", t, curr);
}

static inline void
flush_whitespace(buffer* out, int* ws_count) {

  while(*ws_count > 0) {
    buffer_puts(out, " ");
    --(*ws_count);
  }
}

static inline int
parse_next_of_x(tokenizer* t, token* tok, int fail_unk) {
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

static inline int
mem_tokenizers_join(cpp_file* org,
                    cpp_file* inj,
                    cpp_file* result,
                    int first,
                    off_t lastpos) {
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

  for(;;) {
    if(!(ret = tokenizer_next(&inj->t, &tok)) || tok.type == TT_EOF)
      break;

    emit_token(result->f, &tok, inj->t.buf);
    ++cnt;
  }

  while(tokenizer_ftello(&org->t) < lastpos) {
    ret = tokenizer_next(&org->t, &tok);
    last++;
  }

  diff = cnt - ((int)last - (int)first);

  for(;;) {
    if(!(ret = tokenizer_next(&org->t, &tok)) || tok.type == TT_EOF)
      break;

    emit_token(result->f, &tok, org->t.buf);
  }

  result->f = memstream_reopen(result->f, &result->buf, &result->len);
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
cpp_bp(int type) {
  static const int list[] = {
      /*0x20, 0x10,   0x400,  0x400,  0x800,  0x800,  0x200,  0x200, 0x400,
      0x400, 0x100, 0x40, 0x80, 0x4000, 0x1000, 0x1000, 0x2000, 0x2000,
      0x2000, 0x8000, 0,     0x4000*/
      TTENT(TT_LOR, 1 << 4),     TTENT(TT_LAND, 1 << 5),
      TTENT(TT_BOR, 1 << 6),     TTENT(TT_XOR, 1 << 7),
      TTENT(TT_BAND, 1 << 8),    TTENT(TT_EQ, 1 << 9),
      TTENT(TT_NEQ, 1 << 9),     TTENT(TT_LTE, 1 << 10),
      TTENT(TT_GTE, 1 << 10),    TTENT(TT_LT, 1 << 10),
      TTENT(TT_GT, 1 << 10),     TTENT(TT_SHL, 1 << 11),
      TTENT(TT_SHR, 1 << 11),    TTENT(TT_PLUS, 1 << 12),
      TTENT(TT_MINUS, 1 << 12),  TTENT(TT_MUL, 1 << 13),
      TTENT(TT_DIV, 1 << 13),    TTENT(TT_MOD, 1 << 13),
      TTENT(TT_NEG, 1 << 14),    TTENT(TT_LNOT, 1 << 14),
      TTENT(TT_LPAREN, 1 << 15), TTENT(TT_RPAREN, 0),

      // TTENT(TT_RPAREN, 1 << 15),
      // TTENT(TT_LPAREN, 0),
  };

  if(TTINT(type) < sizeof(list) / sizeof(list[0]))
    return list[TTINT(type)];

  return 0;
}

static inline void
free_visited(char* visited[]) {
  for(size_t i = 0; i < MAX_RECURSION; i++)
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

      case 'x': {
        if(scan_xint(lit + 3, &ret))
          break;
      }

      default: return lit[2];
    }

  return ret;
}

static inline int
was_visited(const char* name, char* visited[], unsigned rec_level) {
  int x;

  for(x = rec_level; x >= 0; --x)
    if(!str_diff(visited[x], name))
      return 1;

  return 0;
}
