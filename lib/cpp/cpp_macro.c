#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"

hashmap cpp_macros = HASHMAP_INIT();

cpp_macro*
cpp_macro_add(char* name, bool is_objlike, cpp_token* body) {
  cpp_macro* m;

  if((m = alloc_zero(sizeof(cpp_macro)))) {
    m->name = name;
    m->is_objlike = is_objlike;
    m->body = body;
    hashmap_put(&cpp_macros, name, m);
  }

  return m;
}

void
cpp_macro_define(char* name, char* buf) {
  cpp_file* file = cpp_file_new("<built-in>", 1, buf);
  cpp_token* tok = cpp_tokenize(file);

  cpp_macro_add(name, true, tok);

  /* while(tok)
     tok = cpp_token_free(tok);*/

  cpp_file_free(cpp_current_file);
  cpp_current_file = 0;
}

/* Otherwise, do nothing and return false. */
bool
cpp_macro_expand(cpp_token** rest, cpp_token* tok) {
  if(cpp_hideset_contains(tok->hideset, tok->loc, tok->len))
    return false;

  cpp_macro* m;

  if(!(m = cpp_macro_find(tok)))
    return false;

  /* Built-in dynamic macro application such as __LINE__ */
  if(m->handler) {
    *rest = m->handler(tok);
    (*rest)->next = tok->next;
    return true;
  }

  /* Object-like macro application */
  if(m->is_objlike) {
    cpp_hideset* hs = cpp_hideset_union(tok->hideset, cpp_hideset_new(m->name));
    cpp_token* body = cpp_hideset_add(m->body, hs);

    for(cpp_token* t = body; t->kind != TK_EOF; t = t->next)
      t->origin = tok;

    *rest = cpp_token_append(body, tok->next);
    (*rest)->at_bol = tok->at_bol;
    (*rest)->has_space = tok->has_space;
    return true;
  }

  /* If a funclike macro token is not followed by an argument list,
     treat it as a normal identifier. */
  if(!cpp_equal(tok->next, "("))
    return false;

  /* Function-like macro application */
  cpp_token* macro_token = tok;
  cpp_macro_arg* args = cpp_read_macro_args(&tok, tok, m->params, m->va_args_name);
  cpp_token* rparen = tok;

  /* Tokens that consist a func-like macro invocation may have different
     hidesets, and if that's the case, it's not clear what the hideset
     for the new tokens should be. We take the interesection of the
     macro token and the closing parenthesis and use it as a new hideset
     as explained in the Dave Prossor's algorithm. */
  cpp_hideset* hs = cpp_hideset_intersection(macro_token->hideset, rparen->hideset);
  hs = cpp_hideset_union(hs, cpp_hideset_new(m->name));

  cpp_token* body = cpp_subst(m->body, args);
  body = cpp_hideset_add(body, hs);

  for(cpp_token* t = body; t->kind != TK_EOF; t = t->next)
    t->origin = macro_token;

  *rest = cpp_token_append(body, tok->next);
  (*rest)->at_bol = macro_token->at_bol;
  (*rest)->has_space = macro_token->has_space;
  return true;
}

cpp_macro*
cpp_macro_find(cpp_token* tok) {
  if(tok->kind != TK_IDENT)
    return NULL;

  return hashmap_get2(&cpp_macros, tok->loc, tok->len);
}

void
cpp_macro_undef(char* name) {
  hashmap_delete(&cpp_macros, name);
}
