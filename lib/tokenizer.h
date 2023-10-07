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

enum tokenizer_result { TOKENIZER_ERROR = -1, TOKENIZER_EOF = -2 };

enum markertype { MT_SINGLELINE_COMMENT_START = 0, MT_MULTILINE_COMMENT_START = 1, MT_MULTILINE_COMMENT_END = 2, MT_MAX = MT_MULTILINE_COMMENT_END };

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

typedef struct token_s {
  enum tokentype type;
  uint32 line;
  uint32 column;
  int64 value;
} token;

enum tokenizer_flags {
  TF_PARSE_STRINGS = 1 << 0,
  TF_PARSE_WIDE_STRINGS = 1 << 1,
};

typedef struct tokenizer_s {
  buffer* input;
  uint32 line;
  uint32 column;
  int flags;
  int custom_count;
  int peeking;
  const char* custom_tokens[MAX_CUSTOM_TOKENS];
  char buf[MAX_TOK_LEN];
  size_t bufsize;
  tokenizer_charbuf chb;
  const char* marker[MT_MAX + 1];
  const char* filename;
  token peek_token;
} tokenizer;

int64 tokenizer_ftello(tokenizer* t);
int tokenizer_getc(tokenizer* t);
int tokenizer_get_flags(tokenizer* t);
void tokenizer_init(tokenizer* t, buffer* in, int flags);
int tokenizer_next(tokenizer* t, struct token_s* out);
int tokenizer_peek(tokenizer* t);
int tokenizer_peek_token(tokenizer* t, struct token_s* tok);
int tokenizer_read_until(tokenizer* t, const char* marker, int stop_at_nl);
void tokenizer_register_custom_token(tokenizer* t, int tokentype, const char* str);
void tokenizer_register_marker(tokenizer* t, enum markertype mt, const char* marker);
int tokenizer_rewind(tokenizer* t);
void tokenizer_set_filename(tokenizer* t, const char* fn);
void tokenizer_set_flags(tokenizer* t, int flags);
int tokenizer_skip_chars(tokenizer* t, const char* chars, int* count);
void tokenizer_skip_until(tokenizer* t, const char* marker);
int tokenizer_ungetc(tokenizer* t, int c);

#endif /* defined(TOKENIZER_H) */
/** @} */
