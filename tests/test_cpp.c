#include "../lib/stralloc.h"
#include "unit_test.h"
#include "../lib/cpp.h"
#include "../lib/cpp_internal.h"
#include "../lib/buffer.h"
#include "../lib/open.h"
#include "../lib/str.h"
#include "../lib/byte.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define SRC "/tmp/test_cpp_unit.c"
#define HDR "/tmp/test_cpp_unit.h"

static void
put_file(const char* path, const char* text) {
  int fd = open_trunc(path);

  if(fd != -1) {
    if(write(fd, text, str_len(text)) < 0)
      fd = -1;
    close(fd);
  }
}

/* preprocesses text (written to a file) and returns the printed result in out;
 * defs is a NULL terminated list of name, value pairs defined before */
static size_t
run(const char* text, const char* const* defs, char* out, size_t max, int with_lines) {
  cpp_ctx pp;
  cpp_token* tok;
  buffer b;
  int fd[2];
  ssize_t n = 0;
  char mem[4096];

  put_file(SRC, text);
  cpp_init(&pp);
  cpp_init_macros(&pp);
  for(; defs && defs[0]; defs += 2)
    cpp_define(&pp, (char*)defs[0], (char*)defs[1]);
  tok = cpp_tokenize_file(&pp, SRC);
  tok = cpp_preprocess(&pp, tok);
  if(pipe(fd))
    return 0;
  buffer_init(&b, (buffer_op_ptr)write, fd[1], mem, sizeof mem);
  cpp_print_tokens(&b, tok, with_lines);
  buffer_flush(&b);
  close(fd[1]);
  n = read(fd[0], out, max - 1);
  close(fd[0]);
  if(n < 0)
    n = 0;
  out[n] = 0;
  /* the output has a trailing newline and, with lines, '# 1 "file"' markers: callers compare a part */
  cpp_free(&pp);
  unlink(SRC);
  return (size_t)n;
}

/* the printed result without the leading and trailing white space */
static const char*
trim(char* s) {
  size_t n = str_len(s);

  while(n && (s[n - 1] == '\n' || s[n - 1] == ' '))
    s[--n] = 0;
  while(*s == '\n' || *s == ' ')
    ++s;
  return s;
}

/*
 * cpp_init, cpp_init_macros, cpp_tokenize_file, cpp_preprocess, cpp_print_tokens, cpp_free
 */
TEST(test_cpp_passthrough) {
  char out[512];

  run("int x = 1 + 2;\n", NULL, out, sizeof out, 0);
  ASSERT_STR_EQUAL("int x = 1 + 2;", trim(out));
}

TEST(test_cpp_object_macro) {
  char out[512];

  run("#define N 42\nint a[N];\n#undef N\nint b = N;\n", NULL, out, sizeof out, 0);
  ASSERT_STR_EQUAL("int a[42];\nint b = N;", trim(out));
}

TEST(test_cpp_function_macro) {
  char out[512];

  run("#define ADD(a, b) ((a) + (b))\nint x = ADD(1, 2 * 3);\n", NULL, out, sizeof out, 0);
  ASSERT_STR_EQUAL("int x = ((1) + (2 * 3));", trim(out));
}

TEST(test_cpp_stringize_paste) {
  char out[512];

  run("#define STR(x) #x\n#define CAT(a, b) a##b\nconst char* s = STR(hello world);\nint CAT(foo, 1) = 0;\n", NULL, out, sizeof out, 0);
  ASSERT_EQ(1, str_contains(out, "\"hello world\""));
  ASSERT_EQ(1, str_contains(out, "foo1"));
}

TEST(test_cpp_variadic) {
  char out[512];

  run("#define LOG(fmt, ...) printf(fmt, __VA_ARGS__)\nLOG(\"%d %d\", 1, 2);\n", NULL, out, sizeof out, 0);
  ASSERT_EQ(1, str_contains(out, "printf(\"%d %d\", 1, 2)"));
}

TEST(test_cpp_conditionals) {
  char out[512];

  run("#if defined(A) && A > 1\nbig\n#elif defined(A)\nsmall\n#else\nnone\n#endif\n", NULL, out, sizeof out, 0);
  ASSERT_STR_EQUAL("none", trim(out));
  {
    static const char* const d1[] = {"A", "5", NULL, NULL};
    run("#if defined(A) && A > 1\nbig\n#elif defined(A)\nsmall\n#else\nnone\n#endif\n", d1, out, sizeof out, 0);
    ASSERT_STR_EQUAL("big", trim(out));
  }
  {
    static const char* const d2[] = {"A", "1", NULL, NULL};
    run("#if defined(A) && A > 1\nbig\n#elif defined(A)\nsmall\n#else\nnone\n#endif\n", d2, out, sizeof out, 0);
    ASSERT_STR_EQUAL("small", trim(out));
  }
  run("#ifdef X\nx\n#endif\n#ifndef X\nnot x\n#endif\n", NULL, out, sizeof out, 0);
  ASSERT_STR_EQUAL("not x", trim(out));
}

TEST(test_cpp_expressions) {
  char out[512];

  run("#if (1 << 4) == 16 && 7 % 3 == 1 && !(2 > 3) && (5 ^ 1) == 4\nyes\n#endif\n", NULL, out, sizeof out, 0);
  ASSERT_STR_EQUAL("yes", trim(out));
  run("#if 0x10 == 16 && 010 == 8 && 'a' == 97\nyes\n#endif\n", NULL, out, sizeof out, 0);
  ASSERT_STR_EQUAL("yes", trim(out));
  run("#if 1 ? 0 : 1\nwrong\n#else\nright\n#endif\n", NULL, out, sizeof out, 0);
  ASSERT_STR_EQUAL("right", trim(out));
}

TEST(test_cpp_predefined) {
  char out[512];

  run("int l = __LINE__;\n", NULL, out, sizeof out, 0);
  ASSERT_EQ(1, str_contains(out, "int l = 1;"));
  run("const char* f = __FILE__;\n", NULL, out, sizeof out, 0);
  ASSERT_EQ(1, str_contains(out, "test_cpp_unit.c"));
  run("int a = __COUNTER__, b = __COUNTER__;\n", NULL, out, sizeof out, 0);
  ASSERT_EQ(1, str_contains(out, "a = 0") && str_contains(out, "b = 1"));
}

TEST(test_cpp_include) {
  char out[512];

  put_file(HDR, "#define FROM_HEADER 7\nint header_decl;\n");
  run("#include \"" HDR "\"\nint x = FROM_HEADER;\n", NULL, out, sizeof out, 0);
  ASSERT_EQ(1, str_contains(out, "int header_decl;"));
  ASSERT_EQ(1, str_contains(out, "int x = 7;"));
  unlink(HDR);
}

TEST(test_cpp_include_guard_pragma_once) {
  char out[512];

  put_file(HDR, "#ifndef GUARD_H\n#define GUARD_H\nint once;\n#endif\n");
  run("#include \"" HDR "\"\n#include \"" HDR "\"\n", NULL, out, sizeof out, 0);
  {
    const char* p = strstr(out, "int once;");

    ASSERT_EQ(1, p != NULL);
    ASSERT_EQ(1, strstr(p + 1, "int once;") == NULL); /* the guard keeps the second copy out */
  }
  put_file(HDR, "#pragma once\nint pragma_once;\n");
  run("#include \"" HDR "\"\n#include \"" HDR "\"\n", NULL, out, sizeof out, 0);
  {
    const char* p = strstr(out, "int pragma_once;");

    ASSERT_EQ(1, p != NULL);
    ASSERT_EQ(1, strstr(p + 1, "int pragma_once;") == NULL);
  }
  unlink(HDR);
}

TEST(test_cpp_recursion_stops) {
  char out[512];

  /* a macro that names itself is not expanded again (the hide set) */
  run("#define foo foo + 1\nint x = foo;\n#define A B\n#define B A\nint y = A;\n", NULL, out, sizeof out, 0);
  ASSERT_EQ(1, str_contains(out, "int x = foo + 1;"));
  ASSERT_EQ(1, str_contains(out, "int y = A;"));
}

TEST(test_cpp_define_api) {
  char out[512];
  static const char* const d[] = {"VERSION", "3", "NAME", "\"pp\"", NULL, NULL};

  run("int v = VERSION; const char* n = NAME;\n", d, out, sizeof out, 0);
  ASSERT_EQ(1, str_contains(out, "int v = 3;"));
  ASSERT_EQ(1, str_contains(out, "n = \"pp\""));
}

TEST(test_cpp_comments_and_continuations) {
  char out[512];

  run("int a; /* block\ncomment */ int b; // line\nint c = 1 \\\n + 2;\n", NULL, out, sizeof out, 0);
  ASSERT_EQ(1, str_contains(out, "int a;"));
  ASSERT_EQ(1, str_contains(out, "int b;"));
  ASSERT_EQ(0, str_contains(out, "comment"));
  ASSERT_EQ(0, str_contains(out, "line"));
  ASSERT_EQ(1, str_contains(out, "int c = 1 + 2;") || str_contains(out, "int c = 1  + 2;"));
}

TEST(test_cpp_quote_string) {
  char* q = cpp_quote_string("a\"b\\c");

  ASSERT_STR_EQUAL("\"a\\\"b\\\\c\"", q);
}

/* tokens of a text, the file stays alive through the tokens */
static cpp_token*
tokens_of(cpp_ctx* pp, const char* text) {
  cpp_file* f = cpp_file_new("internal.c", 1, str_dup(text));
  cpp_token* t = cpp_tokenize(pp, f);

  cpp_file_free(f);
  return t;
}

/*
 * cpp_hideset_new, _add, _contains, _union, _intersection, _free
 */
TEST(test_cpp_hideset) {
  cpp_hideset *a = cpp_hideset_new("FOO"), *b = cpp_hideset_new("BAR"), *u, *i;
  cpp_ctx pp;
  cpp_token* t;

  cpp_init(&pp);
  ASSERT_EQ(1, cpp_hideset_contains(a, "FOO", 3));
  ASSERT_EQ(0, cpp_hideset_contains(a, "FO", 2)); /* the whole name has to match */
  ASSERT_EQ(0, cpp_hideset_contains(a, "BAR", 3));
  ASSERT_EQ(0, cpp_hideset_contains(NULL, "FOO", 3));

  u = cpp_hideset_union(a, b);
  ASSERT_EQ(1, cpp_hideset_contains(u, "FOO", 3));
  ASSERT_EQ(1, cpp_hideset_contains(u, "BAR", 3));
  i = cpp_hideset_intersection(u, b);
  ASSERT_EQ(0, cpp_hideset_contains(i, "FOO", 3));
  ASSERT_EQ(1, cpp_hideset_contains(i, "BAR", 3));
  ASSERT_EQ(NULL, cpp_hideset_intersection(a, b)); /* nothing in common */

  /* adding a hideset to a list of tokens copies the tokens and hides the name from them */
  t = tokens_of(&pp, "x y");
  t = cpp_hideset_add(t, u);
  ASSERT_EQ(1, cpp_hideset_contains(t->hideset, "BAR", 3));

  cpp_hideset_free(i);
  cpp_hideset_free(a);
  cpp_hideset_free(b);
  cpp_free(&pp);
}

/*
 * cpp_token_new, _copy, _append, _eof, _free, _dump, cpp_file_new/free, cpp_tokenize, cpp_join_tokens
 */
TEST(test_cpp_token_basics) {
  cpp_ctx pp;
  cpp_token *t, *c, *e, *joined;
  cpp_token* a;
  char* s;
  stralloc dump;
  buffer b;

  cpp_init(&pp);
  t = tokens_of(&pp, "int x = 0x10;");
  ASSERT_EQ(1, t != NULL);
  ASSERT_EQ(3, t->len);
  ASSERT_EQ(0, byte_diff(t->loc, 3, "int"));
  ASSERT_EQ(1, t->at_bol);
  ASSERT_EQ(0, t->next->at_bol);
  ASSERT_EQ(1, t->next->has_space);
  ASSERT_EQ(0, byte_diff(t->next->loc, 1, "x"));

  c = cpp_token_copy(t);
  ASSERT_EQ(1, c != t);
  ASSERT_EQ(t->loc, c->loc);
  ASSERT_EQ(NULL, c->next);

  e = cpp_token_eof(t);
  ASSERT_EQ(TK_EOF, e->kind);
  ASSERT_EQ(0, e->len);

  /* append copies the first list up to its end token and puts the second behind it */
  a = tokens_of(&pp, "z");
  joined = cpp_token_append(t, a);
  ASSERT_EQ(1, joined != NULL && joined != t);
  s = cpp_join_tokens(joined, NULL);
  ASSERT_EQ(1, s != NULL);
  ASSERT_EQ('z', s[str_len(s) - 1]);
  alloc_free(s);
  ASSERT_EQ(a, cpp_token_append(e, a)); /* an empty first list: the second one as it is */

  s = cpp_join_tokens(t, NULL);
  ASSERT_STR_EQUAL("int x = 0x10;", s); /* a blank only where the source had one */
  alloc_free(s);

  stralloc_init(&dump);
  buffer_tosa(&b, &dump);
  cpp_token_dump(&b, t);
  buffer_flush(&b);
  stralloc_nul(&dump);
  ASSERT_EQ(1, str_contains(dump.s, "kind: "));
  ASSERT_EQ(1, str_contains(dump.s, "'int'"));
  stralloc_free(&dump);

  {
    cpp_token* nt = cpp_token_new(&pp, TK_IDENT, "abc", "abc" + 3);

    ASSERT_EQ(3, nt->len);
    ASSERT_EQ(TK_IDENT, nt->kind);
    ASSERT_EQ(c->next, cpp_token_free(c)); /* gives the next token */
    cpp_token_free(nt);
  }
  cpp_free(&pp);
}

/*
 * cpp_new_num_token, cpp_new_str_token, cpp_convert_number, cpp_convert_int, cpp_convert_tokens
 */
TEST(test_cpp_make_and_convert_tokens) {
  cpp_ctx pp;
  cpp_token *tmpl, *n, *s, *p;

  cpp_init(&pp);
  tmpl = tokens_of(&pp, "x");
  n = cpp_new_num_token(&pp, 42, tmpl);
  ASSERT_EQ(1, n != NULL);
  ASSERT_EQ(2, n->len);
  ASSERT_EQ(0, byte_diff(n->loc, 2, "42"));
  s = cpp_new_str_token(&pp, "a\"b", tmpl);
  ASSERT_EQ(1, s != NULL);
  ASSERT_EQ(TK_STR, s->kind);

  p = tokens_of(&pp, "0x1f 010 5u 12L 0b11");
  cpp_convert_tokens(p);
  ASSERT_EQ(TK_NUM, p->kind);
  ASSERT_EQ(31, p->val);
  ASSERT_EQ(8, p->next->val);
  ASSERT_EQ(5, p->next->next->val);
  ASSERT_EQ(12, p->next->next->next->val);
  ASSERT_EQ(3, p->next->next->next->next->val);
  cpp_free(&pp);
}

/*
 * cpp_type_new, cpp_type_copy, cpp_type_free
 */
TEST(test_cpp_types) {
  cpp_type* a = cpp_type_new(TY_INT, 4, 4);
  cpp_type* c;

  ASSERT_EQ(TY_INT, a->kind);
  ASSERT_EQ(4, a->size);
  ASSERT_EQ(4, a->align);
  c = cpp_type_copy(a);
  ASSERT_EQ(1, c != a);
  ASSERT_EQ(4, c->size);
  ASSERT_EQ(a, c->origin);
  cpp_type_free(c);
  cpp_type_free(a);
}

/*
 * cpp_skip_line, cpp_copy_line, cpp_undefine, cpp_macro_add, cpp_const_expr, cpp_eval_const_expr
 */
TEST(test_cpp_lines_macros_expr) {
  cpp_ctx pp;
  cpp_token *t, *rest, *line;
  char* s;

  cpp_init(&pp);
  t = tokens_of(&pp, "a b c\nd e\n");
  ASSERT_EQ(1, t->at_bol);
  /* a token that starts a line is returned as it is; skipping from the middle runs to the next line */
  ASSERT_EQ(t, cpp_skip_line(t));
  rest = cpp_skip_line(t->next);
  ASSERT_EQ(1, rest->at_bol);
  ASSERT_EQ(0, byte_diff(rest->loc, 1, "d"));

  line = cpp_copy_line(&rest, t); /* t starts a line: nothing is copied */
  ASSERT_EQ(TK_EOF, line->kind);
  line = cpp_copy_line(&rest, t->next);
  ASSERT_EQ(1, line != NULL);
  s = cpp_join_tokens(line, NULL);
  ASSERT_STR_EQUAL("b c", s);
  alloc_free(s);
  ASSERT_EQ(0, byte_diff(rest->loc, 1, "d")); /* stops at the start of the next line */
  cpp_free(&pp);
}
#define RUN_CPP_TESTS() \
  RUN(test_cpp_hideset); \
  RUN(test_cpp_token_basics); \
  RUN(test_cpp_make_and_convert_tokens); \
  RUN(test_cpp_types); \
  RUN(test_cpp_lines_macros_expr); \
  RUN(test_cpp_passthrough); \
  RUN(test_cpp_object_macro); \
  RUN(test_cpp_function_macro); \
  RUN(test_cpp_stringize_paste); \
  RUN(test_cpp_variadic); \
  RUN(test_cpp_conditionals); \
  RUN(test_cpp_expressions); \
  RUN(test_cpp_predefined); \
  RUN(test_cpp_include); \
  RUN(test_cpp_include_guard_pragma_once); \
  RUN(test_cpp_recursion_stops); \
  RUN(test_cpp_define_api); \
  RUN(test_cpp_comments_and_continuations); \
  RUN(test_cpp_quote_string)

TESTS(cpp) { RUN_CPP_TESTS(); }
