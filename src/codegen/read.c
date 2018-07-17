#include "ast.h"
#include "codegen.h"

extern void _CodegenError(mpc_state_t *state, const char *fmt, ...);

void _CodegenContextCodegenReadInt(CodegenContextRef ctx, ASTExprRef expr) {

    g_assert(ctx != NULL);
    g_assert(expr != NULL);

    LLVMValueRef alloca = g_hash_table_lookup(ctx->vars, expr->data);

    if (!alloca) {
        _CodegenError(expr->state,
                      "Variable \"%s\" is not defined",
                      (char *) expr->data);
        exit(EXIT_FAILURE);
    }

    LLVMValueRef scanf_args[] = { ctx->scanf_read_int_fmt, alloca };
    LLVMBuildCall(
            ctx->builder,
            ctx->scanf_fn,
            scanf_args,
            2,
            ""
    );
}
