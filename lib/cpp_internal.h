/* This file implements the C preprocessor.

   The preprocessor takes a list of tokens as an input and returns a
   new list of tokens as an output.

   The preprocessing language is designed in such a way that that's
   guaranteed to stop even if there is a recursive macro.
   Informally speaking, a macro is applied only once for each token.
   That is, if a macro token T appears in a result of direct or
   indirect macro expansion of T, T won't be expanded any further.
   For example, if T is defined as U, and U is defined as T, then
   token T is expanded to U and then to T and the macro expansion
   stops at that point.

   To achieve the above behavior, we attach for each token a set of
   macro names from which the token is expanded. The set is called
   "hideset". cpp_hideset is initially empty, and every time we expand a
   macro, the macro name is added to the resulting tokens' hidesets.

   The above macro expansion algorithm is explained in this document
   written by Dave Prossor, which is used as a basis for the
   standard's wording:
   https://github.com/rui314/chibicc/wiki/cpp.algo.pdf */

#include "uint64.h"
#include "uint32.h"
#include "bool.h"
#include "hashmap.h"
#include "strarray.h"
#include "cpp.h"
#include "case.h"
#include "str.h"
#include "utf8.h"
#include "alloc.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

/* cpp_token*/
typedef enum {
  TK_IDENT,   /* Identifiers */
  TK_PUNCT,   /* Punctuators */
  TK_KEYWORD, /* Keywords */
  TK_STR,     /* String literals */
  TK_NUM,     /* Numeric literals */
  TK_PP_NUM,  /* Preprocessing numbers */
  TK_EOF,     /* End-of-file markers */
} cpp_token_kind;

struct cpp_file {
  char* name;
  int file_no;
  char* contents;

  /* For #line directive*/
  char* display_name;
  int line_delta;
};

/* cpp_token type*/
struct cpp_token {
  cpp_token_kind kind; /* cpp_token kind */
  cpp_token* next;     /* Next token */
  int64 val;           /* If kind is TK_NUM, its value */
  long double fval;    /* If kind is TK_NUM, its value */
  char* loc;           /* cpp_token location */
  size_t len;          /* cpp_token length */
  struct cpp_type* ty; /* Used if TK_NUM or TK_STR */
  char* str;           /* String literal contents including terminating '\0' */

  cpp_file* file;       /* Source location */
  char* filename;       /* Filename */
  int line_no;          /* Line number */
  int line_delta;       /* Line number */
  bool cpp_at_bol;      /* True if this token is at beginning of line */
  bool cpp_has_space;   /* True if this token follows a space character */
  cpp_hideset* hideset; /* For macro expansion */
  cpp_token* origin;    /* If this is expanded from a macro, the original token */
};

typedef enum {
  TY_VOID,
  TY_BOOL,
  TY_CHAR,
  TY_SHORT,
  TY_INT,
  TY_LONG,
  TY_FLOAT,
  TY_DOUBLE,
  TY_LDOUBLE,
  TY_ENUM,
  TY_PTR,
  TY_FUNC,
  TY_ARRAY,
  TY_VLA, /* variable-length array */
  TY_STRUCT,
  TY_UNION,
} cpp_type_kind;

struct cpp_type {
  cpp_type_kind kind;
  size_t size;             /* sizeof() value */
  size_t align;            /* alignment */
  bool is_unsigned;        /* unsigned or signed */
  bool is_atomic;          /* true if _Atomic */
  struct cpp_type* origin; /* for type compatibility check */

  /* Pointer-to or array-of type. We intentionally use the same member*/
  /* to represent pointer/array duality in C.*/
  /**/
  /* In many contexts in which a pointer is expected, we examine this*/
  /* member instead of "kind" member to determine whether a type is a*/
  /* pointer or not. That means in many contexts "array of T" is*/
  /* naturally handled as if it were "pointer to T", as required by*/
  /* the C spec.*/
  struct cpp_type* base;

  /* Declaration*/
  struct cpp_token* name;
  struct cpp_token* name_pos;

  /* Array*/
  size_t array_len;

  /* Variable-length array*/
  struct cpp_node* vla_len; /* # of elements */
  struct cpp_obj* vla_size; /* sizeof() value */

  /* Struct*/
  struct cpp_member* members;
  bool is_flexible;
  bool is_packed;

  /* Function type*/
  struct cpp_type* return_ty;
  struct cpp_type* params;
  bool is_variadic;
  struct cpp_type* next;
};

struct cpp_macro_param {
  struct cpp_macro_param* next;
  char* name;
};

struct cpp_macro_arg {
  struct cpp_macro_arg* next;
  char* name;
  bool is_va_args;
  struct cpp_token* tok;
};

struct cpp_macro {
  char* name;
  bool is_objlike; /* Object-like or function-like */
  struct cpp_macro_param* params;
  char* va_args_name;
  struct cpp_token* body;
  macro_handler_fn* handler;
};

/* `#if` can be nested, so we use a stack to manage nested `#if`s.*/
struct cpp_cond_incl {
  struct cpp_cond_incl* next;
  enum { IN_THEN, IN_ELIF, IN_ELSE } ctx;
  struct cpp_token* tok;
  bool included;
};

struct cpp_hideset {
  struct cpp_hideset* next;
  char* name;
};

extern hashmap cpp_macros;
extern cpp_cond_incl* cond_incl;
extern hashmap cpp_pragma_once;
extern int cpp_include_next_idx;
extern strarray cpp_include_paths;
extern char* cpp_base_file;
extern cpp_file** cpp_input_files;
extern cpp_file* cpp_current_file;
extern bool cpp_at_bol, cpp_has_space;

extern cpp_type *cpp_ty_void, *cpp_ty_bool, *cpp_ty_char, *cpp_ty_short, *cpp_ty_int, *cpp_ty_long, *cpp_ty_uchar, *cpp_ty_ushort, *cpp_ty_uint, *cpp_ty_ulong, *cpp_ty_float, *cpp_ty_double, *cpp_ty_ldouble;

cpp_token* cpp_preprocess2(cpp_token* tok);
cpp_macro* cpp_find_macro(cpp_token* tok);
void cpp_verror_at(char*, char*, int, char*, char*, va_list ap);
cpp_type* cpp_new_type(cpp_type_kind, int, int);
size_t cpp_display_width(char*, size_t);
char* cpp_read_file(char*);

static inline bool
cpp_in_range(uint32* range, uint32 c) {
  for(size_t i = 0; range[i] != -1; i += 2)
    if(range[i] <= c && c <= range[i + 1])
      return true;

  return false;
}

/* Takes a printf-style cpp_format string and returns a formatted string.*/
static inline char*
cpp_format(char* fmt, ...) {
  char* buf;
  size_t buflen;
  FILE* out = open_memstream(&buf, &buflen);

  va_list ap;
  va_start(ap, fmt);
  vfprintf(out, fmt, ap);
  va_end(ap);
  fclose(out);
  return buf;
}

/* Consumes the current token if it matches `op`.*/
static inline bool
cpp_equal(cpp_token* tok, char* op) {
  return byte_equal(tok->loc, tok->len, op) && op[tok->len] == '\0';
}

/* Ensure that the current token is `op`.*/
static inline cpp_token*
cpp_skip(cpp_token* tok, char* op) {
  if(!cpp_equal(tok, op))
    cpp_error_tok(tok, "expected '%s'", op);

  return tok->next;
}

static inline bool
cpp_consume(cpp_token** rest, cpp_token* tok, char* str) {
  if(cpp_equal(tok, str)) {
    *rest = tok->next;
    return true;
  }

  *rest = tok;
  return false;
}

static inline cpp_type*
cpp_array_of(cpp_type* base, int len) {
  cpp_type* ty = cpp_new_type(TY_ARRAY, base->size * len, base->align);

  ty->base = base;
  ty->array_len = len;

  return ty;
}

static inline bool
cpp_is_keyword(cpp_token* tok) {
  static hashmap map;

  if(map.capacity == 0) {
    static char* kw[] = {
        "return",    "if",       "else",   "for",    "while",    "int",           "sizeof",   "char",     "struct",        "union",    "short",      "long",
        "void",      "typedef",  "_Bool",  "enum",   "static",   "goto",          "break",    "continue", "switch",        "case",     "default",    "extern",
        "_Alignof",  "_Alignas", "do",     "signed", "unsigned", "const",         "volatile", "auto",     "register",      "restrict", "__restrict", "__restrict__",
        "_Noreturn", "float",    "double", "typeof", "asm",      "_Thread_local", "__thread", "_Atomic",  "__attribute__",
    };

    for(int i = 0; i < sizeof(kw) / sizeof(*kw); i++)
      hashmap_put(&map, kw[i], (void*)1);
  }

  return hashmap_get2(&map, tok->loc, tok->len);
}

/* Replaces \r or \r\n with \n. */
static inline void
cpp_canonicalize_newline(char* p) {
  size_t i = 0, j = 0;

  while(p[i]) {
    if(p[i] == '\r' && p[i + 1] == '\n') {
      i += 2;
      p[j++] = '\n';
    } else if(p[i] == '\r') {
      i++;
      p[j++] = '\n';
    } else {
      p[j++] = p[i++];
    }
  }

  p[j] = '\0';
}

/* Removes backslashes followed by a newline. */
static inline void
cpp_remove_backslash_newline(char* p) {
  size_t i = 0, j = 0;

  /* We want to keep the number of newline characters so that
     the logical line number matches the physical one.
     This counter maintain the number of newlines we have removed. */
  size_t n = 0;

  while(p[i]) {
    if(p[i] == '\\' && p[i + 1] == '\n') {
      i += 2;
      n++;
    } else if(p[i] == '\n') {
      p[j++] = p[i++];

      for(; n > 0; n--)
        p[j++] = '\n';
    } else {
      p[j++] = p[i++];
    }
  }

  for(; n > 0; n--)
    p[j++] = '\n';

  p[j] = '\0';
}

static inline int
cpp_from_hex(char c) {
  if('0' <= c && c <= '9')
    return c - '0';

  if('a' <= c && c <= 'f')
    return c - 'a' + 10;

  return c - 'A' + 10;
}

static inline wchar_t
cpp_read_universal_char(char* p, size_t len) {
  wchar_t c = 0;

  for(size_t i = 0; i < len; i++) {
    if(!isxdigit(p[i]))
      return 0;

    c = (c << 4) | cpp_from_hex(p[i]);
  }

  return c;
}

/* Replace \u or \U escape sequences with corresponding UTF-8 bytes. */
static inline void
cpp_convert_universal_chars(char* p) {
  char* q = p;

  while(*p) {
    if(str_start(p, "\\u")) {
      wchar_t c = cpp_read_universal_char(p + 2, 4);

      if(c) {
        p += 6;
        q += wc_to_u8(q, c);
      } else {
        *q++ = *p++;
      }
    } else if(str_start(p, "\\U")) {
      wchar_t c = cpp_read_universal_char(p + 2, 8);

      if(c) {
        p += 10;
        q += wc_to_u8(q, c);
      } else {
        *q++ = *p++;
      }
    } else if(p[0] == '\\') {
      *q++ = *p++;
      *q++ = *p++;
    } else {
      *q++ = *p++;
    }
  }

  *q = '\0';
}

/* The definition of the numeric literal at the preprocessing stage
   is more relaxed than the definition of that at the later stages.
   In order to handle that, a numeric literal is tokenized as a
   "pp-number" token first and then converted to a regular number
   token after preprocessing.

   This function converts a pp-number token to a regular number token. */
static inline void
cpp_convert_number(cpp_token* tok) {
  /* Try to parse as an integer constant. */
  if(cpp_convert_int(tok))
    return;

  /* If it's not an integer, it must be a floating point constant. */
  char* end;
  long double val = strtold(tok->loc, &end);

  cpp_type* ty;
  if(*end == 'f' || *end == 'F') {
    ty = cpp_ty_float;
    end++;
  } else if(*end == 'l' || *end == 'L') {
    ty = cpp_ty_ldouble;
    end++;
  } else {
    ty = cpp_ty_double;
  }

  if(tok->loc + tok->len != end)
    cpp_error_tok(tok, "invalid numeric constant");

  tok->kind = TK_NUM;
  tok->fval = val;
  tok->ty = ty;
}

static inline void
cpp_convert_tokens(cpp_token* tok) {
  for(cpp_token* t = tok; t->kind != TK_EOF; t = t->next) {
    if(cpp_is_keyword(t))
      t->kind = TK_KEYWORD;
    else if(t->kind == TK_PP_NUM)
      cpp_convert_number(t);
  }
}

// Initialize line info for all tokens.
static inline void
cpp_add_line_numbers(cpp_token* tok) {
  char* p = cpp_current_file->contents;
  size_t n = 1;

  do {
    if(p == tok->loc) {
      tok->line_no = n;
      tok = tok->next;
    }

    if(*p == '\n')
      n++;

  } while(*p++);
}

/* Create a new token. */
static inline cpp_token*
cpp_new_token(cpp_token_kind kind, char* start, char* end) {
  cpp_token* tok = alloc_zero(sizeof(cpp_token));

  tok->kind = kind;
  tok->loc = start;
  tok->len = end - start;
  tok->file = cpp_current_file;
  tok->filename = cpp_current_file->display_name;
  tok->cpp_at_bol = cpp_at_bol;
  tok->cpp_has_space = cpp_has_space;

  cpp_at_bol = cpp_has_space = false;
  return tok;
}

static inline int
cpp_read_escaped_char(char** new_pos, char* p) {
  if('0' <= *p && *p <= '7') {
    /* Read an octal number. */
    int c = *p++ - '0';

    if('0' <= *p && *p <= '7') {
      c = (c << 3) + (*p++ - '0');

      if('0' <= *p && *p <= '7')
        c = (c << 3) + (*p++ - '0');
    }

    *new_pos = p;
    return c;
  }

  if(*p == 'x') {
    /* Read a hexadecimal number. */
    p++;

    if(!isxdigit(*p))
      cpp_error_at(p, "invalid hex escape sequence");

    int c = 0;

    for(; isxdigit(*p); p++)
      c = (c << 4) + cpp_from_hex(*p);

    *new_pos = p;
    return c;
  }

  *new_pos = p + 1;

  /* Escape sequences are defined using themselves here. E.g.
     '\n' is implemented using '\n'. This tautological definition
     works because the compiler that compiles our compiler knows
     what '\n' actually is. In other words, we "inherit" the ASCII
     code of '\n' from the compiler that compiles our compiler,
     so we don't have to teach the actual code here.

     This fact has huge implications not only for the correctness
     of the compiler but also for the security of the generated code.
     For more info, read "Reflections on Trusting Trust" by Ken Thompson.
     https://github.com/rui314/chibicc/wiki/thompson1984.pdf */
  switch(*p) {
    case 'a': return '\a';
    case 'b': return '\b';
    case 't': return '\t';
    case 'n': return '\n';
    case 'v': return '\v';
    case 'f': return '\f';
    case 'r': return '\r';
    /* [GNU] \e for the ASCII escape character is a GNU C extension. */
    case 'e': return 27;
    default: return *p;
  }
}

/* Find a closing double-quote. */
static inline char*
cpp_string_literal_end(char* p) {
  char* start = p;

  for(; *p != '"'; p++) {
    if(*p == '\n' || *p == '\0')
      cpp_error_at(start, "unclosed string literal");

    if(*p == '\\')
      p++;
  }

  return p;
}

static inline cpp_token*
cpp_read_string_literal(char* start, char* quote) {
  char* end = cpp_string_literal_end(quote + 1);
  char* buf = alloc_zero(end - quote);
  size_t len = 0;

  for(char* p = quote + 1; p < end;) {
    if(*p == '\\')
      buf[len++] = cpp_read_escaped_char(&p, p + 1);
    else
      buf[len++] = *p++;
  }

  cpp_token* tok = cpp_new_token(TK_STR, start, end + 1);
  tok->ty = cpp_array_of(cpp_ty_char, len + 1);
  tok->str = buf;
  return tok;
}

static inline cpp_token*
cpp_read_char_literal(char* start, char* quote, cpp_type* ty) {
  char* p = quote + 1;

  if(*p == '\0')
    cpp_error_at(start, "unclosed char literal");

  wchar_t c;

  if(*p == '\\')
    c = cpp_read_escaped_char(&p, p + 1);
  else
    p += u8_to_wc(&c, p);

  size_t n = str_chr(p, '\'');

  if(!p[n])
    cpp_error_at(p, "unclosed char literal");

  cpp_token* tok = cpp_new_token(TK_NUM, start, (p + n) + 1);
  tok->val = c;
  tok->ty = ty;
  return tok;
}

// [https://www.sigbus.info/n1570#D] C11 allows not only ASCII but
// some multibyte characters in certan Unicode ranges to be used in an
// identifier.
//
// This function returns true if a given character is acceptable as
// the first character of an identifier.
//
// For example, ¾ (U+00BE) is a valid identifier because characters in
// 0x00BE-0x00C0 are allowed, while neither ⟘ (U+27D8) nor '　'
// (U+3000, full-width space) are allowed because they are out of range.
static inline bool
cpp_is_ident1(uint32 c) {
  static uint32 range[] = {
      '_',     '_',     'a',     'z',     'A',     'Z',     '$',     '$',     0x00A8,  0x00A8,  0x00AA,  0x00AA,  0x00AD,  0x00AD,  0x00AF,  0x00AF,  0x00B2,  0x00B5,
      0x00B7,  0x00BA,  0x00BC,  0x00BE,  0x00C0,  0x00D6,  0x00D8,  0x00F6,  0x00F8,  0x00FF,  0x0100,  0x02FF,  0x0370,  0x167F,  0x1681,  0x180D,  0x180F,  0x1DBF,
      0x1E00,  0x1FFF,  0x200B,  0x200D,  0x202A,  0x202E,  0x203F,  0x2040,  0x2054,  0x2054,  0x2060,  0x206F,  0x2070,  0x20CF,  0x2100,  0x218F,  0x2460,  0x24FF,
      0x2776,  0x2793,  0x2C00,  0x2DFF,  0x2E80,  0x2FFF,  0x3004,  0x3007,  0x3021,  0x302F,  0x3031,  0x303F,  0x3040,  0xD7FF,  0xF900,  0xFD3D,  0xFD40,  0xFDCF,
      0xFDF0,  0xFE1F,  0xFE30,  0xFE44,  0xFE47,  0xFFFD,  0x10000, 0x1FFFD, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD, 0x40000, 0x4FFFD, 0x50000, 0x5FFFD, 0x60000, 0x6FFFD,
      0x70000, 0x7FFFD, 0x80000, 0x8FFFD, 0x90000, 0x9FFFD, 0xA0000, 0xAFFFD, 0xB0000, 0xBFFFD, 0xC0000, 0xCFFFD, 0xD0000, 0xDFFFD, 0xE0000, 0xEFFFD, -1,
  };

  return cpp_in_range(range, c);
}

// Returns true if a given character is acceptable as a non-first
// character of an identifier.
static inline bool
cpp_is_ident2(uint32 c) {
  static uint32 range[] = {
      '0',
      '9',
      '$',
      '$',
      0x0300,
      0x036F,
      0x1DC0,
      0x1DFF,
      0x20D0,
      0x20FF,
      0xFE20,
      0xFE2F,
      -1,
  };

  return cpp_is_ident1(c) || cpp_in_range(range, c);
}

/* Read an identifier and returns the length of it.
   If p does not point to a valid identifier, 0 is returned. */
static inline size_t
cpp_read_ident(char* start) {
  char* p = start;
  wchar_t c;

  p += u8_to_wc(&c, p);

  if(!cpp_is_ident1(c))
    return 0;

  for(;;) {
    char* q = p + u8_to_wc(&c, p);

    if(!cpp_is_ident2(c))
      return p - start;

    p = q;
  }
}

/* Read a punctuator token from p and returns its length. */
static inline size_t
cpp_read_punct(char* p) {
  static char* kw[] = {
      "<<=", ">>=", "...", "==", "!=", "<=", ">=", "->", "+=", "-=", "*=", "/=", "++", "--", "%=", "&=", "|=", "^=", "&&", "||", "<<", ">>", "##",
  };

  for(size_t i = 0; i < sizeof(kw) / sizeof(*kw); i++)
    if(str_start(p, kw[i]))
      return str_len(kw[i]);

  return ispunct(*p) ? 1 : 0;
}
