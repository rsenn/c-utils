#include "../cpp_internal.h"
#include "../thread.h"
#include "../alloc.h"

static thread_local cpp_ctx* cpp_ctx_current;

cpp_ctx*
cpp_ctx_new(void) {
  cpp_ctx* ctx = alloc_zero(sizeof(cpp_ctx));

  ctx->macro_ptr = &ctx->macro_list;
  ctx->expr_scope = &ctx->root_scope;

  return ctx;
}

void
cpp_ctx_free(cpp_ctx* ctx) {
  strarray_free(&ctx->inc_array);
  strarray_free(&ctx->inc_paths);

  if(cpp_ctx_current == ctx)
    cpp_ctx_current = 0;

  alloc_free(ctx);
}

cpp_ctx*
cpp_ctx_get(void) {
  if(!cpp_ctx_current)
    cpp_ctx_current = cpp_ctx_new();

  return cpp_ctx_current;
}

void
cpp_ctx_use(cpp_ctx* ctx) {
  cpp_ctx_current = ctx;
}
