#include "../cpp_internal.h"
#include "../str.h"
#include <ctype.h>

/* Initialize line info for all tokens. */
static void
add_line_numbers(cpp_ctx* pp, cpp_file* file, cpp_token* tok) {
  char* p = pp->cur_file->contents;
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

/* Tokenize a given string and returns new tokens. */
cpp_token*
cpp_tokenize(cpp_ctx* pp, cpp_file* file) {
  char* p = file->contents;
  cpp_token head = {0};
  cpp_token* cur = &head;

  pp->cur_file = file;

  pp->bol = true;
  pp->has_sp = 0;

  while(*p) {
    size_t ident_len;
    size_t punct_len;
    /* Skip line comments. */
    if(str_start(p, "//")) {
      char* q = p;

      p += 2;

      while(*p != '\n')
        p++;

      pp->has_sp += p - q;
      continue;
    }

    /* Skip block comments. */
    if(str_start(p, "/*")) {
      size_t q = str_find(p + 2, "*/");

      if(!p[q + 2])
        cpp_error_at(pp, p, "unclosed block comment");

      p += q + 2 + 2;
      pp->has_sp += q + 2 + 2;
      continue;
    }

    /* Skip newline. */
    if(*p == '\n') {
      p++;
      pp->bol = true;
      pp->has_sp++;
      continue;
    }

    /* Skip whitespace characters. */
    if(isspace(*p)) {
      p++;
      pp->has_sp++;
      continue;
    }

    /* Numeric literal */
    if(isdigit(*p) || (*p == '.' && isdigit(p[1]))) {
      char* q = p++;

      for(;;) {
        if(p[0] && p[1] && str_chr("eEpP", p[0]) < 4 && str_chr("+-", p[1]) < 2)
          p += 2;
        else if(isalnum(*p) || *p == '.')
          p++;
        else
          break;
      }

      cur = cur->next = cpp_token_new(pp, TK_PP_NUM, q, p);
      continue;
    }

    /* String literal */
    if(*p == '"') {
      cur = cur->next = cpp_read_string_literal(pp, p, p);
      p += cur->len;
      continue;
    }

    /* UTF-8 string literal */
    if(str_start(p, "u8\"")) {
      cur = cur->next = cpp_read_string_literal(pp, p, p + 2);
      p += cur->len;
      continue;
    }

    /* UTF-16 string literal */
    /*if(str_start(p, "u\"")) {
      cur = cur->next = read_utf16_string_literal(p, p + 1);
      p += cur->len;
      continue;
    }*/

    /* Wide string literal */
    /*if(str_start(p, "L\"")) {
      cur = cur->next = read_utf32_string_literal(p, p + 1, cpp_ty_int);
      p += cur->len;
      continue;
    }*/

    /* UTF-32 string literal */
    /*if(str_start(p, "U\"")) {
      cur = cur->next = read_utf32_string_literal(p, p + 1, cpp_ty_uint);
      p += cur->len;
      continue;
    }*/

    /* Character literal */
    if(*p == '\'') {
      cur = cur->next = cpp_read_char_literal(pp, p, p, cpp_ty_int);
      cur->val = (char)cur->val;
      p += cur->len;
      continue;
    }

    /* UTF-16 character literal */
    if(str_start(p, "u'")) {
      cur = cur->next = cpp_read_char_literal(pp, p, p + 1, cpp_ty_ushort);
      cur->val &= 0xffff;
      p += cur->len;
      continue;
    }

    /* Wide character literal */
    if(str_start(p, "L'")) {
      cur = cur->next = cpp_read_char_literal(pp, p, p + 1, cpp_ty_int);
      p += cur->len;
      continue;
    }

    /* UTF-32 character literal */
    if(str_start(p, "U'")) {
      cur = cur->next = cpp_read_char_literal(pp, p, p + 1, cpp_ty_uint);
      p += cur->len;
      continue;
    }

    /* Identifier or keyword */
    ident_len = cpp_read_ident(p);

    if(ident_len) {
      cur = cur->next = cpp_token_new(pp, TK_IDENT, p, p + ident_len);
      p += cur->len;
      continue;
    }

    /* Punctuators */
    punct_len = cpp_read_punct(p);

    if(punct_len) {
      cur = cur->next = cpp_token_new(pp, TK_PUNCT, p, p + punct_len);
      p += cur->len;
      continue;
    }

    cpp_error_at(pp, p, "invalid token");
  }

  cur = cur->next = cpp_token_new(pp, TK_EOF, p, p);
  add_line_numbers(pp, file, head.next);
  return head.next;
}

/* the basic types; plain objects, since bcc32 has no compound literals */
static cpp_type ty_void = {TY_VOID, 1, 1}, ty_bool = {TY_BOOL, 1, 1}, ty_char = {TY_CHAR, 1, 1},
                ty_short = {TY_SHORT, 2, 2}, ty_int = {TY_INT, 4, 4}, ty_long = {TY_LONG, 8, 8},
                ty_uchar = {TY_CHAR, 1, 1, true}, ty_ushort = {TY_SHORT, 2, 2, true}, ty_uint = {TY_INT, 4, 4, true},
                ty_ulong = {TY_LONG, 8, 8, true}, ty_float = {TY_FLOAT, 4, 4}, ty_double = {TY_DOUBLE, 8, 8},
                ty_ldouble = {TY_LDOUBLE, 16, 16};

cpp_type *cpp_ty_void = &ty_void, *cpp_ty_bool = &ty_bool, *cpp_ty_char = &ty_char, *cpp_ty_short = &ty_short,
         *cpp_ty_int = &ty_int, *cpp_ty_long = &ty_long, *cpp_ty_uchar = &ty_uchar, *cpp_ty_ushort = &ty_ushort,
         *cpp_ty_uint = &ty_uint, *cpp_ty_ulong = &ty_ulong, *cpp_ty_float = &ty_float, *cpp_ty_double = &ty_double,
         *cpp_ty_ldouble = &ty_ldouble;
