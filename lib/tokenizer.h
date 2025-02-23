/**
 * @defgroup   tokenizer
 * @brief      TOKENIZER module.
 * @{
 */

#ifndef TOKENIZER_H
#define TOKENIZER_H

#define MAX_TOK_LEN 4096
#define MAX_UNGETC 8

#include "buffer.h"
#include "uint32.h"
#include "uint64.h"

typedef struct tokenizer_charbuf_s {
  int buf[MAX_UNGETC];
  size_t cnt, buffered;
} tokenizer_charbuf;

enum tokenizer_result {
  TOKENIZER_ERROR = -1,
  TOKENIZER_EOF = -2,
};

enum markertype {
  MT_SINGLELINE_COMMENT_START = 0,
  MT_MULTILINE_COMMENT_START = 1,
  MT_MULTILINE_COMMENT_END = 2,
  MT_MAX = MT_MULTILINE_COMMENT_END,
};

#define MAX_CUSTOM_TOKENS 32

enum tokentype {
  TT_IDENTIFIER = 1,
  TT_SQSTRING_LIT,
  TT_DQSTRING_LIT,
  TT_ELLIPSIS,
  TT_HEX_INT_LIT,
  TT_OCT_INT_LIT,
  TT_DEC_INT_LIT,
  TT_FLOAT_LIT,
  TT_SEP,
  /* errors and similar */
  TT_UNKNOWN,
  TT_OVERFLOW,
  TT_WIDECHAR_LIT,
  TT_WIDESTRING_LIT,
  TT_EOF,
  TT_CUSTOM = 1000 /* start user defined tokentype values */
};

const char* tokentype_to_str(enum tokentype tt);

typedef struct {
  enum tokentype type;
  uint32 line;
  uint32 column;
  int64 value;
} token;

enum tokenizer_flags {
  TF_PARSE_STRINGS = 1 << 0,
  TF_PARSE_WIDE_STRINGS = 1 << 1,
};

typedef struct {
  buffer* input;
  uint32 line, column;
  tokenizer_charbuf chb;
  const char* marker[MT_MAX + 1];
  const char* filename;
  int flags, peeking;
  token peek_token;
  size_t custom_count;
  const char* custom_tokens[MAX_CUSTOM_TOKENS];
  size_t bufsize;
  char buf[MAX_TOK_LEN];
} tokenizer;

int64 tokenizer_ftello(tokenizer*);
int tokenizer_getc(tokenizer*);
int tokenizer_get_flags(tokenizer*);
void tokenizer_init(tokenizer*, buffer* in, int flags);
int tokenizer_next(tokenizer*, token* out);
int tokenizer_peek(tokenizer*);
int tokenizer_peek_token(tokenizer*, token* tok);
int tokenizer_read_until(tokenizer*, const char* marker, int stop_at_nl);
void tokenizer_register_custom_token(tokenizer*, int tokentype, const char* str);
void tokenizer_register_marker(tokenizer*, enum markertype mt, const char* marker);
int tokenizer_rewind(tokenizer*);
void tokenizer_set_filename(tokenizer*, const char* fn);
void tokenizer_set_flags(tokenizer*, int flags);
int tokenizer_skip_chars(tokenizer*, const char* chars, int* count);
void tokenizer_skip_until(tokenizer*, const char* marker);
int tokenizer_ungetc(tokenizer*, int c);
const char* tokentype_to_str(enum tokentype);

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

static inline int
token_is_char(token* tok, int ch) {
  return tok->type == TT_SEP && tok->value == ch;
}

static inline int
token_is_whitespace(token* token) {
  return token->type == TT_SEP && (token->value == ' ' || token->value == '\t');
}

#endif /* defined(TOKENIZER_H) */
/** @} */
