#include "../cpp.h"
#include "../cpp_internal.h"

static void add_type(cpp_node* node);
static int64_t eval2(cpp_node* node, char*** label);
static int64_t eval_rval(cpp_node* node, char*** label);
static double eval_double(cpp_node* node);
static cpp_node* conditional(cpp_token** rest, cpp_token* tok);
static cpp_node* logor(cpp_token** rest, cpp_token* tok);
static cpp_node* logand(cpp_token** rest, cpp_token* tok);
static cpp_node* bitor (cpp_token * *rest, cpp_token* tok);
static cpp_node* bitxor(cpp_token** rest, cpp_token* tok);
static cpp_node*bitand(cpp_token** rest, cpp_token* tok);
static cpp_node* equality(cpp_token** rest, cpp_token* tok);
static cpp_node* relational(cpp_token** rest, cpp_token* tok);
static cpp_node* shift(cpp_token** rest, cpp_token* tok);
static cpp_node* new_add(cpp_node* lhs, cpp_node* rhs, cpp_token* tok);
static cpp_node* new_sub(cpp_node* lhs, cpp_node* rhs, cpp_token* tok);
static cpp_node* add(cpp_token** rest, cpp_token* tok);
static cpp_node* mul(cpp_token** rest, cpp_token* tok);
static cpp_node* unary(cpp_token** rest, cpp_token* tok);
static cpp_node* cast(cpp_token** rest, cpp_token* tok);

static cpp_scope* scope = &(cpp_scope){};
cpp_obj *locals = 0, *globals = 0;

static bool
is_integer(cpp_type* ty) {
  cpp_type_kind k = ty->kind;
  return k == TY_BOOL || k == TY_CHAR || k == TY_SHORT || k == TY_INT || k == TY_LONG || k == TY_ENUM;
}

static bool
is_flonum(cpp_type* ty) {
  return ty->kind == TY_FLOAT || ty->kind == TY_DOUBLE || ty->kind == TY_LDOUBLE;
}

static bool
is_numeric(cpp_type* ty) {
  return is_integer(ty) || is_flonum(ty);
}

/* Returns true if a given token represents a type. */
static bool
is_typename(cpp_token* tok) {
  static hashmap map;

  if(map.capacity == 0) {
    static char* kw[] = {
        "void",       "_Bool",        "char",      "short",  "int",      "long",   "struct",   "union",         "typedef",  "enum",
        "static",     "extern",       "_Alignas",  "signed", "unsigned", "const",  "volatile", "auto",          "register", "restrict",
        "__restrict", "__restrict__", "_Noreturn", "float",  "double",   "typeof", "inline",   "_Thread_local", "__thread", "_Atomic",
    };

    for(int i = 0; i < sizeof(kw) / sizeof(*kw); i++)
      hashmap_put(&map, kw[i], (void*)1);
  }

  return hashmap_get2(&map, tok->loc, tok->len) /*|| find_typedef(tok)*/;
}

static cpp_node*
new_cast(cpp_node* expr, cpp_type* ty) {
  add_type(expr);

  cpp_node* node;

  if((node = alloc_zero(sizeof(cpp_node)))) {
    node->kind = ND_CAST;
    node->tok = expr->tok;
    node->lhs = expr;
    node->ty = cpp_type_copy(ty);
  }

  return node;
}

static struct cpp_var_scope*
push_scope(char* name) {
  struct cpp_var_scope* sc = alloc_zero(sizeof(cpp_var_scope));
  hashmap_put(&scope->vars, name, sc);
  return sc;
}

static cpp_obj*
new_var(char* name, cpp_type* ty) {
  cpp_obj* var;

  if((var = alloc_zero(sizeof(cpp_obj)))) {
    var->name = name;
    var->ty = ty;
    var->align = ty->align;
    push_scope(name)->var = var;
  }

  return var;
}

static cpp_obj*
new_lvar(char* name, cpp_type* ty) {
  cpp_obj* var;

  if((var = new_var(name, ty))) {
    var->is_local = true;
    var->next = locals;
    locals = var;
  }

  return var;
}

static cpp_type*
pointer_to(cpp_type* base) {
  cpp_type* ty;

  if((ty = cpp_type_new(TY_PTR, 8, 8))) {
    ty->base = base;
    ty->is_unsigned = true;
  }

  return ty;
}

static cpp_node*
new_node(cpp_node_kind kind, cpp_token* tok) {
  cpp_node* node = alloc_zero(sizeof(cpp_node));
  node->kind = kind;
  node->tok = tok;
  return node;
}

static cpp_node*
new_binary(cpp_node_kind kind, cpp_node* lhs, cpp_node* rhs, cpp_token* tok) {
  cpp_node* node = new_node(kind, tok);
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

static cpp_node*
new_unary(cpp_node_kind kind, cpp_node* expr, cpp_token* tok) {
  cpp_node* node = new_node(kind, tok);
  node->lhs = expr;
  return node;
}

static cpp_node*
new_var_node(cpp_obj* var, cpp_token* tok) {
  cpp_node* node = new_node(ND_VAR, tok);
  node->var = var;
  return node;
}

static cpp_node*
new_num(int64_t val, cpp_token* tok) {
  cpp_node* node = new_node(ND_NUM, tok);
  node->val = val;
  return node;
}

static cpp_node*
new_long(int64_t val, cpp_token* tok) {
  cpp_node* node = new_node(ND_NUM, tok);
  node->val = val;
  node->ty = cpp_ty_long;
  return node;
}

static cpp_node*
new_ulong(long val, cpp_token* tok) {
  cpp_node* node = new_node(ND_NUM, tok);
  node->val = val;
  node->ty = cpp_ty_ulong;
  return node;
}

static cpp_obj*
new_gvar(char* name, cpp_type* ty) {
  cpp_obj* var = new_var(name, ty);
  var->next = globals;
  var->is_static = true;
  var->is_definition = true;
  globals = var;
  return var;
}

static char*
new_unique_name(void) {
  static int id = 0;
  return cpp_format(".L..%d", id++);
}

static cpp_obj*
new_anon_gvar(cpp_type* ty) {
  return new_gvar(new_unique_name(), ty);
}

static cpp_obj*
new_string_literal(char* p, cpp_type* ty) {
  cpp_obj* var = new_anon_gvar(ty);
  var->init_data = p;
  return var;
}

static cpp_type*
get_common_type(cpp_type* ty1, cpp_type* ty2) {
  if(ty1->base)
    return pointer_to(ty1->base);

  if(ty1->kind == TY_FUNC)
    return pointer_to(ty1);

  if(ty2->kind == TY_FUNC)
    return pointer_to(ty2);

  if(ty1->kind == TY_LDOUBLE || ty2->kind == TY_LDOUBLE)
    return cpp_ty_ldouble;

  if(ty1->kind == TY_DOUBLE || ty2->kind == TY_DOUBLE)
    return cpp_ty_double;

  if(ty1->kind == TY_FLOAT || ty2->kind == TY_FLOAT)
    return cpp_ty_float;

  if(ty1->size < 4)
    ty1 = cpp_ty_int;

  if(ty2->size < 4)
    ty2 = cpp_ty_int;

  if(ty1->size != ty2->size)
    return (ty1->size < ty2->size) ? ty2 : ty1;

  if(ty2->is_unsigned)
    return ty2;

  return ty1;
}

/* For many binary operators, we implicitly promote operands so that
   both operands have the same type. Any integral type smaller than
   int is always promoted to int. If the type of one operand is larger
   than the other's (e.g. "long" vs. "int"), the smaller operand will
   be promoted to match with the other.

   This operation is called the "usual arithmetic conversion". */
static void
usual_arith_conv(cpp_node** lhs, cpp_node** rhs) {
  cpp_type* ty = get_common_type((*lhs)->ty, (*rhs)->ty);

  *lhs = new_cast(*lhs, ty);
  *rhs = new_cast(*rhs, ty);
}

static void
add_type(cpp_node* node) {
  if(!node || node->ty)
    return;

  add_type(node->lhs);
  add_type(node->rhs);
  add_type(node->cond);
  add_type(node->then);
  add_type(node->els);
  add_type(node->init);
  add_type(node->inc);

  for(cpp_node* n = node->body; n; n = n->next)
    add_type(n);

  for(cpp_node* n = node->args; n; n = n->next)
    add_type(n);

  switch(node->kind) {
    case ND_NUM: node->ty = cpp_ty_int; return;
    case ND_ADD:
    case ND_SUB:
    case ND_MUL:
    case ND_DIV:
    case ND_MOD:
    case ND_BITAND:
    case ND_BITOR:
    case ND_BITXOR:
      usual_arith_conv(&node->lhs, &node->rhs);
      node->ty = node->lhs->ty;
      return;

    case ND_NEG: {
      cpp_type* ty = get_common_type(cpp_ty_int, node->lhs->ty);
      node->lhs = new_cast(node->lhs, ty);
      node->ty = ty;
      return;
    }

    case ND_ASSIGN:
      if(node->lhs->ty->kind == TY_ARRAY)
        cpp_error_tok(node->lhs->tok, "not an lvalue");

      if(node->lhs->ty->kind != TY_STRUCT)
        node->rhs = new_cast(node->rhs, node->lhs->ty);

      node->ty = node->lhs->ty;
      return;

    case ND_EQ:
    case ND_NE:
    case ND_LT:
    case ND_LE:
      usual_arith_conv(&node->lhs, &node->rhs);
      node->ty = cpp_ty_int;
      return;

    case ND_FUNCALL: node->ty = node->func_ty->return_ty; return;

    case ND_NOT:
    case ND_LOGOR:
    case ND_LOGAND: node->ty = cpp_ty_int; return;

    case ND_BITNOT:
    case ND_SHL:
    case ND_SHR: node->ty = node->lhs->ty; return;

    case ND_VAR:
    case ND_VLA_PTR:
      /*node->ty = node->var->ty;*/
      return;

    case ND_COND:
      if(node->then->ty->kind == TY_VOID || node->els->ty->kind == TY_VOID) {
        node->ty = cpp_ty_void;
      } else {
        usual_arith_conv(&node->then, &node->els);
        node->ty = node->then->ty;
      }

      return;

    case ND_COMMA: node->ty = node->rhs->ty; return;

    case ND_MEMBER:
      /*node->ty = node->member->ty;*/
      return;

    case ND_ADDR: {
      cpp_type* ty = node->lhs->ty;
      if(ty->kind == TY_ARRAY)
        node->ty = pointer_to(ty->base);
      else
        node->ty = pointer_to(ty);
      return;
    }

    case ND_DEREF:
      if(!node->lhs->ty->base)
        cpp_error_tok(node->tok, "invalid pointer dereference");

      if(node->lhs->ty->base->kind == TY_VOID)
        cpp_error_tok(node->tok, "dereferencing a void pointer");

      node->ty = node->lhs->ty->base;
      return;

    case ND_STMT_EXPR:
      if(node->body) {
        cpp_node* stmt = node->body;

        while(stmt->next)
          stmt = stmt->next;

        if(stmt->kind == ND_EXPR_STMT) {
          node->ty = stmt->lhs->ty;
          return;
        }
      }

      cpp_error_tok(node->tok, "statement expression returning void is not supported");
      return;

    case ND_LABEL_VAL: node->ty = pointer_to(cpp_ty_void); return;

    case ND_CAS:
      add_type(node->cas_addr);
      add_type(node->cas_old);
      add_type(node->cas_new);
      node->ty = cpp_ty_bool;

      if(node->cas_addr->ty->kind != TY_PTR)
        cpp_error_tok(node->cas_addr->tok, "pointer expected");

      if(node->cas_old->ty->kind != TY_PTR)
        cpp_error_tok(node->cas_old->tok, "pointer expected");

      return;

    case ND_EXCH:
      if(node->lhs->ty->kind != TY_PTR)
        cpp_error_tok(node->cas_addr->tok, "pointer expected");

      node->ty = node->lhs->ty->base;
      return;
  }
}

static int64_t
eval(cpp_node* node) {
  return eval2(node, NULL);
}

/* Evaluate a given node as a constant expression.

   A constant expression is either just a number or ptr+n where ptr
   is a pointer to a global variable and n is a postiive/negative
   number. The latter form is accepted only as an initialization
   expression for a global variable. */
static int64_t
eval2(cpp_node* node, char*** label) {
  add_type(node);

  if(is_flonum(node->ty))
    return eval_double(node);

  switch(node->kind) {
    case ND_ADD: return eval2(node->lhs, label) + eval(node->rhs);
    case ND_SUB: return eval2(node->lhs, label) - eval(node->rhs);
    case ND_MUL: return eval(node->lhs) * eval(node->rhs);

    case ND_DIV:
      if(node->ty->is_unsigned)
        return (uint64_t)eval(node->lhs) / eval(node->rhs);
      return eval(node->lhs) / eval(node->rhs);

    case ND_NEG: return -eval(node->lhs);

    case ND_MOD:
      if(node->ty->is_unsigned)
        return (uint64_t)eval(node->lhs) % eval(node->rhs);
      return eval(node->lhs) % eval(node->rhs);

    case ND_BITAND: return eval(node->lhs) & eval(node->rhs);
    case ND_BITOR: return eval(node->lhs) | eval(node->rhs);
    case ND_BITXOR: return eval(node->lhs) ^ eval(node->rhs);
    case ND_SHL: return eval(node->lhs) << eval(node->rhs);

    case ND_SHR:
      if(node->ty->is_unsigned && node->ty->size == 8)
        return (uint64_t)eval(node->lhs) >> eval(node->rhs);
      return eval(node->lhs) >> eval(node->rhs);

    case ND_EQ: return eval(node->lhs) == eval(node->rhs);
    case ND_NE: return eval(node->lhs) != eval(node->rhs);

    case ND_LT:
      if(node->lhs->ty->is_unsigned)
        return (uint64_t)eval(node->lhs) < eval(node->rhs);
      return eval(node->lhs) < eval(node->rhs);

    case ND_LE:
      if(node->lhs->ty->is_unsigned)
        return (uint64_t)eval(node->lhs) <= eval(node->rhs);
      return eval(node->lhs) <= eval(node->rhs);

    case ND_COND: return eval(node->cond) ? eval2(node->then, label) : eval2(node->els, label);
    case ND_COMMA: return eval2(node->rhs, label);
    case ND_NOT: return !eval(node->lhs);
    case ND_BITNOT: return ~eval(node->lhs);
    case ND_LOGAND: return eval(node->lhs) && eval(node->rhs);
    case ND_LOGOR: return eval(node->lhs) || eval(node->rhs);

    case ND_CAST: {
      int64_t val = eval2(node->lhs, label);

      if(is_integer(node->ty)) {
        switch(node->ty->size) {
          case 1: return node->ty->is_unsigned ? (uint8_t)val : (int8_t)val;
          case 2: return node->ty->is_unsigned ? (uint16_t)val : (int16_t)val;
          case 4: return node->ty->is_unsigned ? (uint32_t)val : (int32_t)val;
        }
      }

      return val;
    }

    case ND_ADDR: return eval_rval(node->lhs, label);
    case ND_LABEL_VAL: *label = &node->unique_label; return 0;

    case ND_MEMBER:
      /*if (!label)
        cpp_error_tok(node->tok, "not a compile-time constant");
      if (node->ty->kind != TY_ARRAY)
        cpp_error_tok(node->tok, "invalid initializer");
      return eval_rval(node->lhs, label) + node->member->offset;*/
      return 0;

    case ND_VAR:
      /*if (!label)
        cpp_error_tok(node->tok, "not a compile-time constant");
      if (node->var->ty->kind != TY_ARRAY && node->var->ty->kind != TY_FUNC)
        cpp_error_tok(node->tok, "invalid initializer");
      *label = &node->var->name;*/
      return 0;

    case ND_NUM: return node->val;
  }

  cpp_error_tok(node->tok, "not a compile-time constant");
}

static int64_t
eval_rval(cpp_node* node, char*** label) {
  switch(node->kind) {
    case ND_VAR:
      /*if (node->var->is_local)
        cpp_error_tok(node->tok, "not a compile-time constant");
      *label = &node->var->name;*/
      return 0;

    case ND_DEREF: return eval2(node->lhs, label);

    case ND_MEMBER:
      /*return eval_rval(node->lhs, label) + node->member->offset;*/
      return 0;
  }

  cpp_error_tok(node->tok, "invalid initializer");
}

static double
eval_double(cpp_node* node) {
  add_type(node);

  if(is_integer(node->ty)) {
    if(node->ty->is_unsigned)
      return (unsigned long)eval(node);

    return eval(node);
  }

  switch(node->kind) {
    case ND_ADD: return eval_double(node->lhs) + eval_double(node->rhs);
    case ND_SUB: return eval_double(node->lhs) - eval_double(node->rhs);
    case ND_MUL: return eval_double(node->lhs) * eval_double(node->rhs);
    case ND_DIV: return eval_double(node->lhs) / eval_double(node->rhs);
    case ND_NEG: return -eval_double(node->lhs);
    case ND_COND: return eval_double(node->cond) ? eval_double(node->then) : eval_double(node->els);
    case ND_COMMA: return eval_double(node->rhs);

    case ND_CAST:
      if(is_flonum(node->lhs->ty))
        return eval_double(node->lhs);

      return eval(node->lhs);

    case ND_NUM: return node->fval;
  }

  cpp_error_tok(node->tok, "not a compile-time constant");
}

static bool
is_const_expr(cpp_node* node) {
  add_type(node);

  switch(node->kind) {
    case ND_ADD:
    case ND_SUB:
    case ND_MUL:
    case ND_DIV:
    case ND_BITAND:
    case ND_BITOR:
    case ND_BITXOR:
    case ND_SHL:
    case ND_SHR:
    case ND_EQ:
    case ND_NE:
    case ND_LT:
    case ND_LE:
    case ND_LOGAND:
    case ND_LOGOR: return is_const_expr(node->lhs) && is_const_expr(node->rhs);

    case ND_COND:
      if(!is_const_expr(node->cond))
        return false;
      return is_const_expr(eval(node->cond) ? node->then : node->els);

    case ND_COMMA: return is_const_expr(node->rhs);

    case ND_NEG:
    case ND_NOT:
    case ND_BITNOT:
    case ND_CAST: return is_const_expr(node->lhs);

    case ND_NUM: return true;
  }

  return false;
}

// Convert op= operators to expressions containing an assignment.
//
// In general, `A op= C` is converted to ``tmp = &A, *tmp = *tmp op B`.
// However, if a given expression is of form `A.x op= C`, the input is
// converted to `tmp = &A, (*tmp).x = (*tmp).x op C` to handle assignments
// to bitfields.
static cpp_node*
to_assign(cpp_node* binary) {
  add_type(binary->lhs);
  add_type(binary->rhs);
  cpp_token* tok = binary->tok;

  // Convert `A.x op= C` to `tmp = &A, (*tmp).x = (*tmp).x op C`.
  if(binary->lhs->kind == ND_MEMBER) {
    cpp_obj* var = new_lvar("", pointer_to(binary->lhs->lhs->ty));

    cpp_node* expr1 = new_binary(ND_ASSIGN, new_var_node(var, tok), new_unary(ND_ADDR, binary->lhs->lhs, tok), tok);

    cpp_node* expr2 = new_unary(ND_MEMBER, new_unary(ND_DEREF, new_var_node(var, tok), tok), tok);
    expr2->member = binary->lhs->member;

    cpp_node* expr3 = new_unary(ND_MEMBER, new_unary(ND_DEREF, new_var_node(var, tok), tok), tok);
    expr3->member = binary->lhs->member;

    cpp_node* expr4 = new_binary(ND_ASSIGN, expr2, new_binary(binary->kind, expr3, binary->rhs, tok), tok);

    return new_binary(ND_COMMA, expr1, expr4, tok);
  }

  // If A is an atomic type, Convert `A op= B` to
  //
  // ({
  //   T1 *addr = &A; T2 val = (B); T1 old = *addr; T1 new;
  //   do {
  //    new = old op val;
  //   } while (!atomic_compare_exchange_strong(addr, &old, new));
  //   new;
  // })
  if(binary->lhs->ty->is_atomic) {
    cpp_node head = {};
    cpp_node* cur = &head;

    cpp_obj* addr = new_lvar("", pointer_to(binary->lhs->ty));
    cpp_obj* val = new_lvar("", binary->rhs->ty);
    cpp_obj* old = new_lvar("", binary->lhs->ty);
    cpp_obj* new = new_lvar("", binary->lhs->ty);

    cur = cur->next = new_unary(ND_EXPR_STMT, new_binary(ND_ASSIGN, new_var_node(addr, tok), new_unary(ND_ADDR, binary->lhs, tok), tok), tok);

    cur = cur->next = new_unary(ND_EXPR_STMT, new_binary(ND_ASSIGN, new_var_node(val, tok), binary->rhs, tok), tok);

    cur = cur->next = new_unary(ND_EXPR_STMT, new_binary(ND_ASSIGN, new_var_node(old, tok), new_unary(ND_DEREF, new_var_node(addr, tok), tok), tok), tok);

    cpp_node* loop = new_node(ND_DO, tok);
    loop->brk_label = new_unique_name();
    loop->cont_label = new_unique_name();

    cpp_node* body = new_binary(ND_ASSIGN, new_var_node(new, tok), new_binary(binary->kind, new_var_node(old, tok), new_var_node(val, tok), tok), tok);

    loop->then = new_node(ND_BLOCK, tok);
    loop->then->body = new_unary(ND_EXPR_STMT, body, tok);

    cpp_node* cas = new_node(ND_CAS, tok);
    cas->cas_addr = new_var_node(addr, tok);
    cas->cas_old = new_unary(ND_ADDR, new_var_node(old, tok), tok);
    cas->cas_new = new_var_node(new, tok);
    loop->cond = new_unary(ND_NOT, cas, tok);

    cur = cur->next = loop;
    cur = cur->next = new_unary(ND_EXPR_STMT, new_var_node(new, tok), tok);

    cpp_node* node = new_node(ND_STMT_EXPR, tok);
    node->body = head.next;
    return node;
  }

  // Convert `A op= B` to ``tmp = &A, *tmp = *tmp op B`.
  cpp_obj* var = new_lvar("", pointer_to(binary->lhs->ty));

  cpp_node* expr1 = new_binary(ND_ASSIGN, new_var_node(var, tok), new_unary(ND_ADDR, binary->lhs, tok), tok);

  cpp_node* expr2 =
      new_binary(ND_ASSIGN, new_unary(ND_DEREF, new_var_node(var, tok), tok), new_binary(binary->kind, new_unary(ND_DEREF, new_var_node(var, tok), tok), binary->rhs, tok), tok);

  return new_binary(ND_COMMA, expr1, expr2, tok);
}

// assign    = conditional (assign-op assign)?
// assign-op = "=" | "+=" | "-=" | "*=" | "/=" | "%=" | "&=" | "|=" | "^="
//           | "<<=" | ">>="
static cpp_node*
assign(cpp_token** rest, cpp_token* tok) {
  cpp_node* node = conditional(&tok, tok);

  if(cpp_equal(tok, "="))
    return new_binary(ND_ASSIGN, node, assign(rest, tok->next), tok);

  if(cpp_equal(tok, "+="))
    return to_assign(new_add(node, assign(rest, tok->next), tok));

  if(cpp_equal(tok, "-="))
    return to_assign(new_sub(node, assign(rest, tok->next), tok));

  if(cpp_equal(tok, "*="))
    return to_assign(new_binary(ND_MUL, node, assign(rest, tok->next), tok));

  if(cpp_equal(tok, "/="))
    return to_assign(new_binary(ND_DIV, node, assign(rest, tok->next), tok));

  if(cpp_equal(tok, "%="))
    return to_assign(new_binary(ND_MOD, node, assign(rest, tok->next), tok));

  if(cpp_equal(tok, "&="))
    return to_assign(new_binary(ND_BITAND, node, assign(rest, tok->next), tok));

  if(cpp_equal(tok, "|="))
    return to_assign(new_binary(ND_BITOR, node, assign(rest, tok->next), tok));

  if(cpp_equal(tok, "^="))
    return to_assign(new_binary(ND_BITXOR, node, assign(rest, tok->next), tok));

  if(cpp_equal(tok, "<<="))
    return to_assign(new_binary(ND_SHL, node, assign(rest, tok->next), tok));

  if(cpp_equal(tok, ">>="))
    return to_assign(new_binary(ND_SHR, node, assign(rest, tok->next), tok));

  *rest = tok;
  return node;
}

// expr = assign ("," expr)?
static cpp_node*
expr(cpp_token** rest, cpp_token* tok) {
  cpp_node* node = assign(&tok, tok);

  if(cpp_equal(tok, ","))
    return new_binary(ND_COMMA, node, expr(rest, tok->next), tok);

  *rest = tok;
  return node;
}

// conditional = logor ("?" expr? ":" conditional)?
static cpp_node*
conditional(cpp_token** rest, cpp_token* tok) {
  cpp_node* cond = logor(&tok, tok);

  if(!cpp_equal(tok, "?")) {
    *rest = tok;
    return cond;
  }

  if(cpp_equal(tok->next, ":")) {
    // [GNU] Compile `a ?: b` as `tmp = a, tmp ? tmp : b`.
    add_type(cond);
    struct cpp_obj* var = new_lvar("", cond->ty);
    cpp_node* lhs = new_binary(ND_ASSIGN, new_var_node(var, tok), cond, tok);
    cpp_node* rhs = new_node(ND_COND, tok);
    rhs->cond = new_var_node(var, tok);
    rhs->then = new_var_node(var, tok);
    rhs->els = conditional(rest, tok->next->next);
    return new_binary(ND_COMMA, lhs, rhs, tok);
  }

  cpp_node* node = new_node(ND_COND, tok);
  node->cond = cond;
  node->then = expr(&tok, tok->next);
  tok = cpp_skip(tok, ":");
  node->els = conditional(rest, tok);
  return node;
}

// logor = logand ("||" logand)*
static cpp_node*
logor(cpp_token** rest, cpp_token* tok) {
  cpp_node* node = logand(&tok, tok);
  while(cpp_equal(tok, "||")) {
    cpp_token* start = tok;
    node = new_binary(ND_LOGOR, node, logand(&tok, tok->next), start);
  }
  *rest = tok;
  return node;
}

// logand = bitor ("&&" bitor)*
static cpp_node*
logand(cpp_token** rest, cpp_token* tok) {
  cpp_node* node = bitor (&tok, tok);
  while(cpp_equal(tok, "&&")) {
    cpp_token* start = tok;
    node = new_binary(ND_LOGAND, node, bitor (&tok, tok->next), start);
  }
  *rest = tok;
  return node;
}

// bitor = bitxor ("|" bitxor)*
static cpp_node* bitor (cpp_token * *rest, cpp_token* tok) {
  cpp_node* node = bitxor(&tok, tok);
  while(cpp_equal(tok, "|")) {
    cpp_token* start = tok;
    node = new_binary(ND_BITOR, node, bitxor(&tok, tok->next), start);
  }
  *rest = tok;
  return node;
}

// bitxor = bitand ("^" bitand)*
static cpp_node*
bitxor(cpp_token** rest, cpp_token* tok) {
  cpp_node* node = bitand(&tok, tok);
  while(cpp_equal(tok, "^")) {
    cpp_token* start = tok;
    node = new_binary(ND_BITXOR, node, bitand(&tok, tok->next), start);
  }
  *rest = tok;
  return node;
}

// bitand = equality ("&" equality)*
static cpp_node*bitand(cpp_token** rest, cpp_token* tok) {
  cpp_node* node = equality(&tok, tok);
  while(cpp_equal(tok, "&")) {
    cpp_token* start = tok;
    node = new_binary(ND_BITAND, node, equality(&tok, tok->next), start);
  }
  *rest = tok;
  return node;
}

// equality = relational ("==" relational | "!=" relational)*
static cpp_node*
equality(cpp_token** rest, cpp_token* tok) {
  cpp_node* node = relational(&tok, tok);

  for(;;) {
    cpp_token* start = tok;

    if(cpp_equal(tok, "==")) {
      node = new_binary(ND_EQ, node, relational(&tok, tok->next), start);
      continue;
    }

    if(cpp_equal(tok, "!=")) {
      node = new_binary(ND_NE, node, relational(&tok, tok->next), start);
      continue;
    }

    *rest = tok;
    return node;
  }
}

// relational = shift ("<" shift | "<=" shift | ">" shift | ">=" shift)*
static cpp_node*
relational(cpp_token** rest, cpp_token* tok) {
  cpp_node* node = shift(&tok, tok);

  for(;;) {
    cpp_token* start = tok;

    if(cpp_equal(tok, "<")) {
      node = new_binary(ND_LT, node, shift(&tok, tok->next), start);
      continue;
    }

    if(cpp_equal(tok, "<=")) {
      node = new_binary(ND_LE, node, shift(&tok, tok->next), start);
      continue;
    }

    if(cpp_equal(tok, ">")) {
      node = new_binary(ND_LT, shift(&tok, tok->next), node, start);
      continue;
    }

    if(cpp_equal(tok, ">=")) {
      node = new_binary(ND_LE, shift(&tok, tok->next), node, start);
      continue;
    }

    *rest = tok;
    return node;
  }
}

// shift = add ("<<" add | ">>" add)*
static cpp_node*
shift(cpp_token** rest, cpp_token* tok) {
  cpp_node* node = add(&tok, tok);

  for(;;) {
    cpp_token* start = tok;

    if(cpp_equal(tok, "<<")) {
      node = new_binary(ND_SHL, node, add(&tok, tok->next), start);
      continue;
    }

    if(cpp_equal(tok, ">>")) {
      node = new_binary(ND_SHR, node, add(&tok, tok->next), start);
      continue;
    }

    *rest = tok;
    return node;
  }
}

// In C, `+` operator is overloaded to perform the pointer arithmetic.
// If p is a pointer, p+n adds not n but sizeof(*p)*n to the value of p,
// so that p+n points to the location n elements (not bytes) ahead of p.
// In other words, we need to scale an integer value before adding to a
// pointer value. This function takes care of the scaling.
static cpp_node*
new_add(cpp_node* lhs, cpp_node* rhs, cpp_token* tok) {
  add_type(lhs);
  add_type(rhs);

  // num + num
  if(is_numeric(lhs->ty) && is_numeric(rhs->ty))
    return new_binary(ND_ADD, lhs, rhs, tok);

  if(lhs->ty->base && rhs->ty->base)
    cpp_error_tok(tok, "invalid operands");

  // Canonicalize `num + ptr` to `ptr + num`.
  if(!lhs->ty->base && rhs->ty->base) {
    cpp_node* tmp = lhs;
    lhs = rhs;
    rhs = tmp;
  }

  // VLA + num
  if(lhs->ty->base->kind == TY_VLA) {
    rhs = new_binary(ND_MUL, rhs, new_var_node(lhs->ty->base->vla_size, tok), tok);
    return new_binary(ND_ADD, lhs, rhs, tok);
  }

  // ptr + num
  rhs = new_binary(ND_MUL, rhs, new_long(lhs->ty->base->size, tok), tok);
  return new_binary(ND_ADD, lhs, rhs, tok);
}

// Like `+`, `-` is overloaded for the pointer type.
static cpp_node*
new_sub(cpp_node* lhs, cpp_node* rhs, cpp_token* tok) {
  add_type(lhs);
  add_type(rhs);

  // num - num
  if(is_numeric(lhs->ty) && is_numeric(rhs->ty))
    return new_binary(ND_SUB, lhs, rhs, tok);

  // VLA + num
  if(lhs->ty->base->kind == TY_VLA) {
    rhs = new_binary(ND_MUL, rhs, new_var_node(lhs->ty->base->vla_size, tok), tok);
    add_type(rhs);
    cpp_node* node = new_binary(ND_SUB, lhs, rhs, tok);
    node->ty = lhs->ty;
    return node;
  }

  // ptr - num
  if(lhs->ty->base && is_integer(rhs->ty)) {
    rhs = new_binary(ND_MUL, rhs, new_long(lhs->ty->base->size, tok), tok);
    add_type(rhs);
    cpp_node* node = new_binary(ND_SUB, lhs, rhs, tok);
    node->ty = lhs->ty;
    return node;
  }

  // ptr - ptr, which returns how many elements are between the two.
  if(lhs->ty->base && rhs->ty->base) {
    cpp_node* node = new_binary(ND_SUB, lhs, rhs, tok);
    node->ty = cpp_ty_long;
    return new_binary(ND_DIV, node, new_num(lhs->ty->base->size, tok), tok);
  }

  cpp_error_tok(tok, "invalid operands");
}

// add = mul ("+" mul | "-" mul)*
static cpp_node*
add(cpp_token** rest, cpp_token* tok) {
  cpp_node* node = mul(&tok, tok);

  for(;;) {
    cpp_token* start = tok;

    if(cpp_equal(tok, "+")) {
      node = new_add(node, mul(&tok, tok->next), start);
      continue;
    }

    if(cpp_equal(tok, "-")) {
      node = new_sub(node, mul(&tok, tok->next), start);
      continue;
    }

    *rest = tok;
    return node;
  }
}

// mul = cast ("*" cast | "/" cast | "%" cast)*
static cpp_node*
mul(cpp_token** rest, cpp_token* tok) {
  cpp_node* node = cast(&tok, tok);

  for(;;) {
    cpp_token* start = tok;

    if(cpp_equal(tok, "*")) {
      node = new_binary(ND_MUL, node, cast(&tok, tok->next), start);
      continue;
    }

    if(cpp_equal(tok, "/")) {
      node = new_binary(ND_DIV, node, cast(&tok, tok->next), start);
      continue;
    }

    if(cpp_equal(tok, "%")) {
      node = new_binary(ND_MOD, node, cast(&tok, tok->next), start);
      continue;
    }

    *rest = tok;
    return node;
  }
}

// cast = "(" type-name ")" cast | unary
static cpp_node*
cast(cpp_token** rest, cpp_token* tok) {
  if(cpp_equal(tok, "(") && is_typename(tok->next)) {
    cpp_token* start = tok;
    cpp_type* ty = 0; // typename(&tok, tok->next);
    tok = cpp_skip(tok, ")");

    // compound literal
    if(cpp_equal(tok, "{"))
      return unary(rest, start);

    // type cast
    cpp_node* node = new_cast(cast(rest, tok), ty);
    node->tok = start;
    return node;
  }

  return unary(rest, tok);
}

// primary = "(" "{" stmt+ "}" ")"
//         | "(" expr ")"
//         | "sizeof" "(" type-name ")"
//         | "sizeof" unary
//         | "_Alignof" "(" type-name ")"
//         | "_Alignof" unary
//         | "_Generic" generic-selection
//         | "__builtin_types_compatible_p" "(" type-name, type-name, ")"
//         | "__builtin_reg_class" "(" type-name ")"
//         | ident
//         | str
//         | num
static cpp_node*
primary(cpp_token** rest, cpp_token* tok) {
  cpp_token* start = tok;

  /* This is a GNU statement expresssion. */
  /* if(cpp_equal(tok, "(") && cpp_equal(tok->next, "{")) {
     cpp_node* node = new_node(ND_STMT_EXPR, tok);
     node->body = compound_stmt(&tok, tok->next->next)->body;
     *rest = cpp_skip(tok, ")");
     return node;
   }*/

  if(cpp_equal(tok, "(")) {
    cpp_node* node = expr(&tok, tok->next);
    *rest = cpp_skip(tok, ")");
    return node;
  }

  /*if(cpp_equal(tok, "sizeof") && cpp_equal(tok->next, "(") && is_typename(tok->next->next)) {
    cpp_type* ty = 0; //typename(&tok, tok->next->next);
    *rest = cpp_skip(tok, ")");

    if(ty->kind == TY_VLA) {
      if(ty->vla_size)
        return new_var_node(ty->vla_size, tok);

      cpp_node* lhs = compute_vla_size(ty, tok);
      cpp_node* rhs = new_var_node(ty->vla_size, tok);
      return new_binary(ND_COMMA, lhs, rhs, tok);
    }

    return new_ulong(ty->size, start);
  }*/

  if(cpp_equal(tok, "sizeof")) {
    cpp_node* node = unary(rest, tok->next);
    add_type(node);
    if(node->ty->kind == TY_VLA)
      return new_var_node(node->ty->vla_size, tok);
    return new_ulong(node->ty->size, tok);
  }

  /*if(cpp_equal(tok, "_Alignof") && cpp_equal(tok->next, "(") && is_typename(tok->next->next)) {
    cpp_type* ty = typename(&tok, tok->next->next);
    *rest = cpp_skip(tok, ")");
    return new_ulong(ty->align, tok);
  }
*/
  if(cpp_equal(tok, "_Alignof")) {
    cpp_node* node = unary(rest, tok->next);
    add_type(node);
    return new_ulong(node->ty->align, tok);
  }

  /*if(cpp_equal(tok, "_Generic"))
    return generic_selection(rest, tok->next);*/

  /*if (cpp_equal(tok, "__builtin_types_compatible_p")) {
       tok = cpp_skip(tok->next, "(");
       cpp_type *t1 = typename(&tok, tok);
       tok = cpp_skip(tok, ",");
       cpp_type *t2 = typename(&tok, tok);
       *rest = cpp_skip(tok, ")");
       return new_num(is_compatible(t1, t2), start);
     }

     if (cpp_equal(tok, "__builtin_reg_class")) {
       tok = cpp_skip(tok->next, "(");
       cpp_type *ty = typename(&tok, tok);
       *rest = cpp_skip(tok, ")");

       if (is_integer(ty) || ty->kind == TY_PTR)
         return new_num(0, start);
       if (is_flonum(ty))
         return new_num(1, start);
       return new_num(2, start);
     }

     if (cpp_equal(tok, "__builtin_compare_and_swap")) {
       cpp_node *node = new_node(ND_CAS, tok);
       tok = cpp_skip(tok->next, "(");
       node->cas_addr = assign(&tok, tok);
       tok = cpp_skip(tok, ",");
       node->cas_old = assign(&tok, tok);
       tok = cpp_skip(tok, ",");
       node->cas_new = assign(&tok, tok);
       *rest = cpp_skip(tok, ")");
       return node;
     }

     if (cpp_equal(tok, "__builtin_atomic_exchange")) {
       cpp_node *node = new_node(ND_EXCH, tok);
       tok = cpp_skip(tok->next, "(");
       node->lhs = assign(&tok, tok);
       tok = cpp_skip(tok, ",");
       node->rhs = assign(&tok, tok);
       *rest = cpp_skip(tok, ")");
       return node;
     }
   */
  /*if(tok->kind == TK_IDENT) {
    cpp_var_scope* sc = find_var(tok);
    *rest = tok->next;

    if(sc && sc->var && sc->var->is_function) {
      if(current_fn)
        strarray_push(&current_fn->refs, sc->var->name);
      else
        sc->var->is_root = true;
    }

    if(sc) {
      if(sc->var)
        return new_var_node(sc->var, tok);
      if(sc->enum_ty)
        return new_num(sc->enum_val, tok);
    }

    if(cpp_equal(tok->next, "("))
      error_tok(tok, "implicit declaration of a function");
    error_tok(tok, "undefined variable");
  }*/

  if(tok->kind == TK_STR) {
    cpp_obj* var = new_string_literal(tok->str, tok->ty);
    *rest = tok->next;
    return new_var_node(var, tok);
  }

  if(tok->kind == TK_NUM) {
    cpp_node* node;
    if(is_flonum(tok->ty)) {
      node = new_node(ND_NUM, tok);
      node->fval = tok->fval;
    } else {
      node = new_num(tok->val, tok);
    }

    node->ty = tok->ty;
    *rest = tok->next;
    return node;
  }

  cpp_error_tok(tok, "expected an expression");
}

// unary = ("+" | "-" | "*" | "&" | "!" | "~") cast
//       | ("++" | "--") unary
//       | "&&" ident
//       | postfix
static cpp_node*
unary(cpp_token** rest, cpp_token* tok) {
  if(cpp_equal(tok, "+"))
    return cast(rest, tok->next);

  if(cpp_equal(tok, "-"))
    return new_unary(ND_NEG, cast(rest, tok->next), tok);

  if(cpp_equal(tok, "&")) {
    cpp_node* lhs = cast(rest, tok->next);
    add_type(lhs);

    /*if(lhs->kind == ND_MEMBER && lhs->member->is_bitfield)
      cpp_error_tok(tok, "cannot take address of bitfield");*/

    return new_unary(ND_ADDR, lhs, tok);
  }

  if(cpp_equal(tok, "*")) {
    // [https://www.sigbus.info/n1570#6.5.3.2p4] This is an oddity
    // in the C spec, but dereferencing a function shouldn't do
    // anything. If foo is a function, `*foo`, `**foo` or `*****foo`
    // are all equivalent to just `foo`.
    cpp_node* node = cast(rest, tok->next);
    add_type(node);
    if(node->ty->kind == TY_FUNC)
      return node;
    return new_unary(ND_DEREF, node, tok);
  }

  if(cpp_equal(tok, "!"))
    return new_unary(ND_NOT, cast(rest, tok->next), tok);

  if(cpp_equal(tok, "~"))
    return new_unary(ND_BITNOT, cast(rest, tok->next), tok);

  /* Read ++i as i+=1 */
  if(cpp_equal(tok, "++"))
    return to_assign(new_add(unary(rest, tok->next), new_num(1, tok), tok));

  /* Read --i as i-=1 */
  if(cpp_equal(tok, "--"))
    return to_assign(new_sub(unary(rest, tok->next), new_num(1, tok), tok));

  /* [GNU] labels-as-values */
  /* if (cpp_equal(tok, "&&")) {
     cpp_node *node = new_node(ND_LABEL_VAL, tok);
     node->label = get_ident(tok->next);
     node->goto_next = gotos;
     gotos = node;
     *rest = tok->next->next;
     return node;
   }
 */

  return primary(rest, tok);
  /* return postfix(rest, tok); */
}

int64
cpp_const_expr(cpp_token** rest, cpp_token* tok) {
  cpp_node* node = conditional(rest, tok);
  return eval(node);

  /**rest = cpp_token_new(TK_EOF, 0, 0);
  return -1;*/
}
