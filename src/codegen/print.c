#include "ast.h"
#include "codegen.h"

extern void _CodegenError(mpc_state_t *state, const char *fmt, ...);

void _CodegenContextCodegenPrint(CodegenContextRef ctx, ASTExprRef expr) {

    g_assert(ctx != NULL);
    g_assert(expr != NULL);

    LLVMValueRef str = LLVMBuildGlobalStringPtr(
            ctx->builder,
            expr->data,
            ""
    );

    LLVMValueRef printf_args[] = { ctx->printf_str_fmt, str };
    LLVMBuildCall(
            ctx->builder,
            ctx->printf_fn,
            printf_args,
            2,
            ""
    );

}

void _CodegenContextCodegenPrintIdentifier(CodegenContextRef ctx, ASTExprRef expr) {

    g_assert(ctx != NULL);
    g_assert(expr != NULL);

    LLVMValueRef alloca = g_hash_table_lookup(ctx->vars, expr->data);

    if (!alloca) {
        _CodegenError(expr->state,
                      "Variable \"%s\" is not defined.",
                      (char *) expr->data);
        exit(EXIT_FAILURE);
    }

    LLVMValueRef load = LLVMBuildLoad(ctx->builder, alloca, "");
    LLVMValueRef printf_args[] = { ctx->printf_int_fmt, load };
    LLVMBuildCall(
            ctx->builder,
            ctx->printf_fn,
            printf_args,
            2,
            ""
    );
}
