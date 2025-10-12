#include "tokenizer.h"
#include "byte.h"
#include "str.h"
#include "util.h"
#include <assert.h>
#include <ctype.h>

/*#define isdigit(c) ((c) >= '0' && (c) <= '9')
#define isspace(c) ((c) == ' ' || (c) == '\r' || (c) == '\n' || (c) == '\t' || (c) == '\v')
#define tolower(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 0x20 : (c))*/

#define TOKENIZER_ERROR -1
#define TOKENIZER_EOF -2

static inline int
has_ul_tail(const char* p) {
  char tail[4];
  int tc = 0, c;

  while(tc < 4) {
    if(!*p)
      break;

    c = tolower(*p);

    if(c == 'u' || c == 'l') {
      tail[tc++] = c;
    } else {
      return 0;
    }

    p++;
  }

  if(tc == 1)
    return 1;

  if(tc == 2) {
    if(!byte_diff(tail, 2, "lu"))
      return 1;

    if(!byte_diff(tail, 2, "ul"))
      return 1;

    if(!byte_diff(tail, 2, "ll"))
      return 1;
  }

  if(tc == 3) {
    if(!byte_diff(tail, 3, "llu"))
      return 1;

    if(!byte_diff(tail, 3, "ull"))
      return 1;
  }

  return 0;
}

static inline int
is_hex_int_literal(const char* s) {
  if(s[0] == '-')
    s++;

  if(s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
    const char* p = s + 2;

    while(*p) {
      if(str_has("0123456789abcdef", tolower(*p))) {
        if(p == s + 2)
          return 0;
        return has_ul_tail(p);
      }

      p++;
    }

    return 1;
  }

  return 0;
}

static inline int
is_plus_or_minus(int c) {
  return c == '-' || c == '+';
}

static inline int
is_dec_int_literal(const char* str) {
  const char* s = str;

  if(is_plus_or_minus(s[0]))
    s++;

  if(s[0] == '0') {
    if(s[1] == 0)
      return 1;

    if(isdigit(s[1]))
      return 0;
  }

  while(*s) {
    if(!isdigit(*s))
      return (s > str && (is_plus_or_minus(str[0]) ? s > str + 1 : 1)) ? has_ul_tail(s) : 0;

    s++;
  }

  return 1;
}

static inline int
is_float_literal(const char* str) {
  const char* s = str;
  int got_dot = 0, got_e = 0, got_digits = 0;

  if(is_plus_or_minus(s[0]))
    s++;

  while(*s) {
    int l = tolower(*s);

    if(*s == '.') {
      if(got_dot)
        return 0;

      got_dot = 1;
    } else if(l == 'f') {
      if(s[1] == 0 && (got_dot || got_e) && got_digits)
        return 1;

      return 0;
    } else if(isdigit(*s)) {
      got_digits = 1;
    } else if(l == 'e') {

      if(!got_digits)
        return 0;

      s++;

      if(is_plus_or_minus(*s))
        s++;

      if(!isdigit(*s))
        return 0;

      got_e = 1;
    } else {
      return 0;
    }

    s++;
  }

  if(got_digits && (got_e || got_dot))
    return 1;

  return 0;
}

static inline int
is_valid_float_until(const char* s, const char* until) {
  int got_digits = 0, got_dot = 0;

  while(s < until) {
    if(isdigit(*s)) {
      got_digits = 1;
    } else if(*s == '.') {
      if(got_dot)
        return 0;

      got_dot = 1;
    } else {
      return 0;
    }
    ++s;
  }

  return got_digits | (got_dot << 1);
}

static inline int
is_oct_int_literal(const char* s) {
  if(s[0] == '-')
    s++;

  if(s[0] != '0')
    return 0;

  while(*s) {
    if(!str_has("01234567", *s))
      return 0;

    s++;
  }

  return 1;
}

static inline int
is_identifier(const char* s) {
  static const char ascmap[128] = {
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0
      /*
      ['0'] = 2, ['1'] = 2, ['2'] = 2, ['3'] = 2, ['4'] = 2, ['5'] = 2,
      ['6'] = 2, ['7'] = 2, ['8'] = 2, ['9'] = 2, ['A'] = 1, ['B'] = 1,
      ['C'] = 1,
      ['D'] = 1, ['E'] = 1, ['F'] = 1, ['G'] = 1, ['H'] = 1, ['I'] = 1,
      ['J'] = 1, ['K'] = 1, ['L'] = 1, ['M'] = 1, ['N'] = 1, ['O'] = 1,
      ['P'] = 1,
      ['Q'] = 1, ['R'] = 1, ['S'] = 1, ['T'] = 1, ['U'] = 1, ['V'] = 1,
      ['W'] = 1, ['X'] = 1, ['Y'] = 1, ['Z'] = 1, ['_'] = 1, ['a'] = 1,
      ['b'] = 1,
      ['c'] = 1, ['d'] = 1, ['e'] = 1, ['f'] = 1, ['g'] = 1, ['h'] = 1,
      ['i'] = 1, ['j'] = 1, ['k'] = 1, ['l'] = 1, ['m'] = 1, ['n'] = 1,
      ['o'] = 1,
      ['p'] = 1, ['q'] = 1, ['r'] = 1, ['s'] = 1, ['t'] = 1, ['u'] = 1,
      ['v'] = 1, ['w'] = 1, ['x'] = 1, ['y'] = 1, ['z'] = 1,
      */
  };

  if((*s) & 128)
    return 0;

  if(ascmap[(unsigned)*s] != 1)
    return 0;

  ++s;

  while(*s) {
    if((*s) & 128)
      return 0;

    if(!ascmap[(unsigned)*s])
      return 0;

    s++;
  }

  return 1;
}

static inline enum tokentype
categorize(const char* s) {
  if(is_hex_int_literal(s))
    return TT_HEX_INT_LIT;

  if(is_dec_int_literal(s))
    return TT_DEC_INT_LIT;

  if(is_oct_int_literal(s))
    return TT_OCT_INT_LIT;

  if(is_float_literal(s))
    return TT_FLOAT_LIT;

  if(is_identifier(s))
    return TT_IDENTIFIER;

  return TT_UNKNOWN;
}

static inline int
is_sep(int c) {
  static const char ascmap[128] = {
      0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
      1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0
      /*
      ['\t'] = 1, ['\n'] = 1, [' '] = 1,  ['!'] = 1, ['\"'] = 1, ['#'] = 1,
      ['%'] = 1, ['&'] = 1, ['\''] = 1, ['('] = 1, [')'] = 1,
      ['*'] = 1,  ['+'] = 1,  [','] = 1,  ['-'] = 1, ['.'] = 1,  ['/'] = 1,
      [':'] = 1, [';'] = 1, ['<'] = 1,  ['='] = 1, ['>'] = 1,
      ['?'] = 1,  ['['] = 1,  ['\\'] = 1, [']'] = 1, ['{'] = 1,  ['|'] = 1,
      ['}'] = 1, ['~'] = 1, ['^'] = 1,
      */
  };

  return !(c & 128) && ascmap[c];
}

static inline int
apply_coords(tokenizer* t, token* out, char* end, int retval) {
  size_t len = end - t->buf;

  out->line = t->line;
  out->column = t->column - len;

  if(len + 1 >= t->bufsize) {
    out->type = TT_OVERFLOW;
    return 0;
  }

  return retval;
}

static inline char*
assign_bufchar(tokenizer* t, char* s, int c) {
  t->column++;
  *s = c;

  return s + 1;
}

static inline int
get_string(tokenizer* t, char quote_char, token* out, int wide) {
  int escaped = 0;
  char *s = t->buf + 1, *end = t->buf + t->bufsize - 2;

  while(s < end) {
    int c;

    if((c = tokenizer_getc(t)) == TOKENIZER_EOF) {
      out->type = TT_EOF;
      *s = 0;
      return apply_coords(t, out, s, 0);
    }

    if(c == '\\') {
      if((c = tokenizer_getc(t)) == '\n')
        continue;

      tokenizer_ungetc(t, c);
      c = '\\';
    }

    if(c == '\n') {
      if(escaped) {
        escaped = 0;
        continue;
      }

      tokenizer_ungetc(t, c);
      out->type = TT_UNKNOWN;
      s = assign_bufchar(t, s, 0);

      return apply_coords(t, out, s, 0);
    }

    if(!escaped) {
      if(c == quote_char) {
        s = assign_bufchar(t, s, c);
        *s = 0;
        // s = assign_bufchar(t, s, 0);

        if(!wide)
          out->type = (quote_char == '"' ? TT_DQSTRING_LIT : TT_SQSTRING_LIT);
        else
          out->type = (quote_char == '"' ? TT_WIDESTRING_LIT : TT_WIDECHAR_LIT);

        return apply_coords(t, out, s, 1);
      }

      if(c == '\\')
        escaped = 1;
    } else {
      escaped = 0;
    }

    s = assign_bufchar(t, s, c);
  }

  t->buf[MAX_TOK_LEN - 1] = 0;
  out->type = TT_OVERFLOW;

  return apply_coords(t, out, s, 0);
}

/* if sequence found, next tokenizer_s call will point after the sequence
 */
static inline int
sequence_follows(tokenizer* t, int c, const char* which) {
  size_t i = 0;

  if(!which || !which[0])
    return 0;

  while(c == which[i]) {
    if(!which[++i])
      break;

    c = tokenizer_getc(t);
  }

  if(!which[i])
    return 1;

  while(i > 0) {
    tokenizer_ungetc(t, c);
    c = which[--i];
  }

  return 0;
}

static inline int
ignore_until(tokenizer* t, const char* marker, int col_advance) {
  int c;

  t->column += col_advance;

  do {
    if((c = tokenizer_getc(t)) == TOKENIZER_EOF)
      return 0;

    if(c == '\n') {
      t->line++;
      t->column = 0;
    } else {
      t->column++;
    }

  } while(!sequence_follows(t, c, marker));

  t->column += str_len(marker) - 1;

  return 1;
}
