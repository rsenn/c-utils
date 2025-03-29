#include "../cpp_internal.h"
#include "../str.h"
#include <ctype.h>

bool cpp_at_bol = false, cpp_has_space = false;
cpp_file* cpp_current_file = 0;

/* Tokenize a given string and returns new tokens. */
cpp_token*
cpp_tokenize(cpp_file* file) {
  cpp_current_file = file;
  char* p = file->contents;
  cpp_token head = {};
  cpp_token* cur = &head;

  cpp_at_bol = true;
  cpp_has_space = false;

  while(*p) {
    /* Skip line comments. */
    if(str_start(p, "//")) {
      p += 2;

      while(*p != '\n')
        p++;

      cpp_has_space = true;
      continue;
    }

    /* Skip block comments. */
    if(str_start(p, "/*")) {
      size_t q = str_find(p + 2, "*/");

      if(!p[q + 2])
        cpp_error_at(p, "unclosed block comment");

      p += q + 2 + 2;
      cpp_has_space = true;
      continue;
    }

    /* Skip newline. */
    if(*p == '\n') {
      p++;
      cpp_at_bol = true;
      cpp_has_space = false;
      continue;
    }

    /* Skip whitespace characters. */
    if(isspace(*p)) {
      p++;
      cpp_has_space = true;
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

      cur = cur->next = cpp_token_new(TK_PP_NUM, q, p);
      continue;
    }

    /* String literal */
    if(*p == '"') {
      cur = cur->next = cpp_read_string_literal(p, p);
      p += cur->len;
      continue;
    }

    /* UTF-8 string literal */
    if(str_start(p, "u8\"")) {
      cur = cur->next = cpp_read_string_literal(p, p + 2);
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
      cur = cur->next = cpp_read_char_literal(p, p, cpp_ty_int);
      cur->val = (char)cur->val;
      p += cur->len;
      continue;
    }

    /* UTF-16 character literal */
    if(str_start(p, "u'")) {
      cur = cur->next = cpp_read_char_literal(p, p + 1, cpp_ty_ushort);
      cur->val &= 0xffff;
      p += cur->len;
      continue;
    }

    /* Wide character literal */
    if(str_start(p, "L'")) {
      cur = cur->next = cpp_read_char_literal(p, p + 1, cpp_ty_int);
      p += cur->len;
      continue;
    }

    /* UTF-32 character literal */
    if(str_start(p, "U'")) {
      cur = cur->next = cpp_read_char_literal(p, p + 1, cpp_ty_uint);
      p += cur->len;
      continue;
    }

    /* Identifier or keyword */
    size_t ident_len = cpp_read_ident(p);

    if(ident_len) {
      cur = cur->next = cpp_token_new(TK_IDENT, p, p + ident_len);
      p += cur->len;
      continue;
    }

    /* Punctuators */
    size_t punct_len = cpp_read_punct(p);

    if(punct_len) {
      cur = cur->next = cpp_token_new(TK_PUNCT, p, p + punct_len);
      p += cur->len;
      continue;
    }

    cpp_error_at(p, "invalid token");
  }

  cur = cur->next = cpp_token_new(TK_EOF, p, p);
  cpp_add_line_numbers(head.next);
  return head.next;
}

cpp_type *cpp_ty_void = &(cpp_type){TY_VOID, 1, 1}, *cpp_ty_bool = &(cpp_type){TY_BOOL, 1, 1}, *cpp_ty_char = &(cpp_type){TY_CHAR, 1, 1},
         *cpp_ty_short = &(cpp_type){TY_SHORT, 2, 2}, *cpp_ty_int = &(cpp_type){TY_INT, 4, 4}, *cpp_ty_long = &(cpp_type){TY_LONG, 8, 8},
         *cpp_ty_uchar = &(cpp_type){TY_CHAR, 1, 1, true}, *cpp_ty_ushort = &(cpp_type){TY_SHORT, 2, 2, true}, *cpp_ty_uint = &(cpp_type){TY_INT, 4, 4, true},
         *cpp_ty_ulong = &(cpp_type){TY_LONG, 8, 8, true}, *cpp_ty_float = &(cpp_type){TY_FLOAT, 4, 4}, *cpp_ty_double = &(cpp_type){TY_DOUBLE, 8, 8},
         *cpp_ty_ldouble = &(cpp_type){TY_LDOUBLE, 16, 16};
