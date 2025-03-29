#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"
#include "../byte.h"
#include "../str.h"
#include <assert.h>

static cpp_string_kind
get_string_kind(cpp_token* tok) {
  if(str_equal(tok->loc, "u8"))
    return STR_UTF8;

  switch(tok->loc[0]) {
    case '"': return STR_NONE;
    case 'u': return STR_UTF16;
    case 'U': return STR_UTF32;
    case 'L': return STR_WIDE;
  }

  assert(0);
}

/* Concatenate adjacent string literals into a single string literal
   as per the C spec. */
void
cpp_join_adjacent_string_literals(cpp_token* tok) {
  /* First pass: If regular string literals are adjacent to wide
     string literals, regular string literals are converted to a wide
     type before concatenation. In this pass, we do the conversion. **/
  for(cpp_token* tok1 = tok; tok1->kind != TK_EOF;) {
    if(tok1->kind != TK_STR || tok1->next->kind != TK_STR) {
      tok1 = tok1->next;
      continue;
    }

    cpp_string_kind kind = get_string_kind(tok1);
    cpp_type* basety = tok1->ty->base;

    for(cpp_token* t = tok1->next; t->kind == TK_STR; t = t->next) {
      cpp_string_kind k = get_string_kind(t);

      if(kind == STR_NONE) {
        kind = k;
        basety = t->ty->base;
      } else if(k != STR_NONE && kind != k) {
        cpp_error_tok(t, "unsupported non-standard concatenation of string literals");
      }
    }

    if(basety->size > 1) {
      assert(0);

      /* XXX: TODO */
      /*for(cpp_token* t = tok1; t->kind == TK_STR; t = t->next)
        if(t->ty->base->size == 1)
          *t = *cpp_tokenize_string_literal(t, basety);*/
    }

    while(tok1->kind == TK_STR)
      tok1 = tok1->next;
  }

  /* Second pass: concatenate adjacent string literals. */
  for(cpp_token* tok1 = tok; tok1->kind != TK_EOF;) {
    if(tok1->kind != TK_STR || tok1->next->kind != TK_STR) {
      tok1 = tok1->next;
      continue;
    }

    cpp_token* tok2 = tok1->next;

    while(tok2->kind == TK_STR)
      tok2 = tok2->next;

    int len = tok1->ty->array_len;

    for(cpp_token* t = tok1->next; t != tok2; t = t->next)
      len = len + t->ty->array_len - 1;

    char* buf = alloc_zero(tok1->ty->base->size * len);
    int i = 0;

    for(cpp_token* t = tok1; t != tok2; t = t->next) {
      byte_copy(buf + i, t->ty->size, t->str);
      i = i + t->ty->size - t->ty->base->size;
    }

    *tok1 = *cpp_token_copy(tok1);
    tok1->ty = cpp_array_of(tok1->ty->base, len);
    tok1->str = buf;
    tok1->next = tok2;
    tok1 = tok2;
  }
}
