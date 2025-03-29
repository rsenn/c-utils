// This file implements the C preprocessor.
//
// The preprocessor takes a list of tokens as an input and returns a
// new list of tokens as an output.
//
// The preprocessing language is designed in such a way that that's
// guaranteed to stop even if there is a recursive macro.
// Informally speaking, a macro is applied only once for each token.
// That is, if a macro token T appears in a result of direct or
// indirect macro expansion of T, T won't be expanded any further.
// For example, if T is defined as U, and U is defined as T, then
// token T is expanded to U and then to T and the macro expansion
// stops at that point.
//
// To achieve the above behavior, we attach for each token a set of
// macro names from which the token is expanded. The set is called
// "hideset". cpp_hideset is initially empty, and every time we expand a
// macro, the macro name is added to the resulting tokens' hidesets.
//
// The above macro expansion algorithm is explained in this document
// written by Dave Prossor, which is used as a basis for the
// standard's wording:
// https://github.com/rui314/chibicc/wiki/cpp.algo.pdf

#include "uint64.h"
#include "bool.h"
#include "hashmap.h"
#include "strarray.h"
#include "cpp.h"
#include "case.h"
#include "str.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

// cpp_token
typedef enum {
  TK_IDENT,   // Identifiers
  TK_PUNCT,   // Punctuators
  TK_KEYWORD, // Keywords
  TK_STR,     // String literals
  TK_NUM,     // Numeric literals
  TK_PP_NUM,  // Preprocessing numbers
  TK_EOF,     // End-of-file markers
} cpp_token_kind;

struct cpp_file {
  char* name;
  int file_no;
  char* contents;

  // For #line directive
  char* display_name;
  int line_delta;
};

// cpp_token type
struct cpp_token {
  cpp_token_kind kind; // cpp_token kind
  cpp_token* next;     // Next token
  int64 val;           // If kind is TK_NUM, its value
  long double fval;    // If kind is TK_NUM, its value
  char* loc;           // cpp_token location
  int len;             // cpp_token length
  struct cpp_type* ty; // Used if TK_NUM or TK_STR
  char* str;           // String literal contents including terminating '\0'

  cpp_file* file;       // Source location
  char* filename;       // Filename
  int line_no;          // Line number
  int line_delta;       // Line number
  bool at_bol;          // True if this token is at beginning of line
  bool has_space;       // True if this token follows a space character
  cpp_hideset* hideset; // For macro expansion
  cpp_token* origin;    // If this is expanded from a macro, the original token
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
  TY_VLA, // variable-length array
  TY_STRUCT,
  TY_UNION,
} cpp_type_kind;

struct cpp_type {
  cpp_type_kind kind;
  int size;                // sizeof() value
  int align;               // alignment
  bool is_unsigned;        // unsigned or signed
  bool is_atomic;          // true if _Atomic
  struct cpp_type* origin; // for type compatibility check

  // Pointer-to or array-of type. We intentionally use the same member
  // to represent pointer/array duality in C.
  //
  // In many contexts in which a pointer is expected, we examine this
  // member instead of "kind" member to determine whether a type is a
  // pointer or not. That means in many contexts "array of T" is
  // naturally handled as if it were "pointer to T", as required by
  // the C spec.
  struct cpp_type* base;

  // Declaration
  struct cpp_token* name;
  struct cpp_token* name_pos;

  // Array
  int array_len;

  // Variable-length array
  struct cpp_node* vla_len; // # of elements
  struct cpp_obj* vla_size; // sizeof() value

  // Struct
  struct cpp_member* members;
  bool is_flexible;
  bool is_packed;

  // Function type
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
  bool is_objlike; // Object-like or function-like
  struct cpp_macro_param* params;
  char* va_args_name;
  struct cpp_token* body;
  macro_handler_fn* handler;
};

// `#if` can be nested, so we use a stack to manage nested `#if`s.
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

extern hashmap macros;
extern cpp_cond_incl* cond_incl;
extern hashmap cpp_pragma_once;
extern int cpp_include_next_idx;
extern strarray cpp_include_paths;
extern char* cpp_base_file;

extern cpp_type* ty_void;
extern cpp_type* ty_bool;

extern cpp_type* ty_char;
extern cpp_type* ty_short;
extern cpp_type* ty_int;
extern cpp_type* ty_long;

extern cpp_type* ty_uchar;
extern cpp_type* ty_ushort;
extern cpp_type* ty_uint;
extern cpp_type* ty_ulong;

extern cpp_type* ty_float;
extern cpp_type* ty_double;
extern cpp_type* ty_ldouble;

cpp_token* cpp_preprocess2(cpp_token* tok);
cpp_macro* cpp_find_macro(cpp_token* tok);
void cpp_verror_at(char*, char*, int, char*, char*, va_list ap);
cpp_type* cpp_new_type(cpp_type_kind, int, int);

// Takes a printf-style cpp_format string and returns a formatted string.
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

// Consumes the current token if it matches `op`.
static inline bool
cpp_equal(cpp_token* tok, char* op) {
  return byte_equal(tok->loc, tok->len, op) && op[tok->len] == '\0';
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

static inline bool
cpp_convert_int(cpp_token* tok) {
  char* p = tok->loc;

  // Read a binary, octal, decimal or hexadecimal number.
  int base = 10;
  if(!case_diffb(p, 2, "0x") && isxdigit(p[2])) {
    p += 2;
    base = 16;
  } else if(!case_diffb(p, 2, "0b") && (p[2] == '0' || p[2] == '1')) {
    p += 2;
    base = 2;
  } else if(*p == '0') {
    base = 8;
  }

  int64_t val = strtoul(p, &p, base);

  // Read U, L or LL suffixes.
  bool l = false;
  bool u = false;

  if(str_start(p, "LLU") || str_start(p, "LLu") || str_start(p, "llU") || str_start(p, "llu") || str_start(p, "ULL") || str_start(p, "Ull") || str_start(p, "uLL") ||
     str_start(p, "ull")) {
    p += 3;
    l = u = true;
  } else if(!case_diffb(p, 2, "lu") || !case_diffb(p, 2, "ul")) {
    p += 2;
    l = u = true;
  } else if(str_start(p, "LL") || str_start(p, "ll")) {
    p += 2;
    l = true;
  } else if(*p == 'L' || *p == 'l') {
    p++;
    l = true;
  } else if(*p == 'U' || *p == 'u') {
    p++;
    u = true;
  }

  if(p != tok->loc + tok->len)
    return false;

  // Infer a type.
  cpp_type* ty;
  if(base == 10) {
    if(l && u)
      ty = ty_ulong;
    else if(l)
      ty = ty_long;
    else if(u)
      ty = (val >> 32) ? ty_ulong : ty_uint;
    else
      ty = (val >> 31) ? ty_long : ty_int;
  } else {
    if(l && u)
      ty = ty_ulong;
    else if(l)
      ty = (val >> 63) ? ty_ulong : ty_long;
    else if(u)
      ty = (val >> 32) ? ty_ulong : ty_uint;
    else if(val >> 63)
      ty = ty_ulong;
    else if(val >> 32)
      ty = ty_long;
    else if(val >> 31)
      ty = ty_uint;
    else
      ty = ty_int;
  }

  tok->kind = TK_NUM;
  tok->val = val;
  tok->ty = ty;
  return true;
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
    ty = ty_float;
    end++;
  } else if(*end == 'l' || *end == 'L') {
    ty = ty_ldouble;
    end++;
  } else {
    ty = ty_double;
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

/*static inline cpp_token*
cpp_tokenize_string_literal(cpp_token* tok, cpp_type* basety) {
  cpp_token* t;

  if(basety->size == 2)
    t = read_utf16_string_literal(tok->loc, tok->loc);
  else
    t = read_utf32_string_literal(tok->loc, tok->loc, basety);

  t->next = tok->next;
  return t;
}*/
