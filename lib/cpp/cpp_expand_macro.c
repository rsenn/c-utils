#include "../cpp.h"
#include "../cpp_internal.h"

// Otherwise, do nothing and return false.
bool
cpp_expand_macro(cpp_token** rest, cpp_token* tok) {
  if(cpp_hideset_contains(tok->hideset, tok->loc, tok->len))
    return false;

  cpp_macro* m = cpp_find_macro(tok);
  if(!m)
    return false;

  // Built-in dynamic macro application such as __LINE__
  if(m->handler) {
    *rest = m->handler(tok);
    (*rest)->next = tok->next;
    return true;
  }

  // Object-like macro application
  if(m->is_objlike) {
    cpp_hideset* hs = cpp_hideset_union(tok->hideset, cpp_new_hideset(m->name));
    cpp_token* body = cpp_add_hideset(m->body, hs);
    for(cpp_token* t = body; t->kind != TK_EOF; t = t->next)
      t->origin = tok;
    *rest = cpp_append(body, tok->next);
    (*rest)->cpp_at_bol = tok->cpp_at_bol;
    (*rest)->cpp_has_space = tok->cpp_has_space;
    return true;
  }

  // If a funclike macro token is not followed by an argument list,
  // treat it as a normal identifier.
  if(!cpp_equal(tok->next, "("))
    return false;

  // Function-like macro application
  cpp_token* macro_token = tok;
  cpp_macro_arg* args = cpp_read_macro_args(&tok, tok, m->params, m->va_args_name);
  cpp_token* rparen = tok;

  // Tokens that consist a func-like macro invocation may have different
  // hidesets, and if that's the case, it's not clear what the hideset
  // for the new tokens should be. We take the interesection of the
  // macro token and the closing parenthesis and use it as a new hideset
  // as explained in the Dave Prossor's algorithm.
  cpp_hideset* hs = cpp_hideset_intersection(macro_token->hideset, rparen->hideset);
  hs = cpp_hideset_union(hs, cpp_new_hideset(m->name));

  cpp_token* body = cpp_subst(m->body, args);
  body = cpp_add_hideset(body, hs);
  for(cpp_token* t = body; t->kind != TK_EOF; t = t->next)
    t->origin = macro_token;
  *rest = cpp_append(body, tok->next);
  (*rest)->cpp_at_bol = macro_token->cpp_at_bol;
  (*rest)->cpp_has_space = macro_token->cpp_has_space;
  return true;
}
