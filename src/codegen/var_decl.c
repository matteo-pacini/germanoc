#include "ast.h"
#include "codegen.h"

extern void _CodegenError(mpc_state_t *state, const char *fmt, ...);

void _CodegenContextCodegenVarDecl(CodegenContextRef ctx, ASTExprRef expr) {

    g_assert(ctx != NULL);
    g_assert(expr != NULL);

    ASTVarDeclRef data = expr->data;

    if (g_hash_table_lookup(ctx->vars, data->name)) {
        _CodegenError(expr->state,
                      "Variable \"%s\" is already defined",
                      data->name);
        exit(EXIT_FAILURE);
    }

    LLVMValueRef alloca = LLVMBuildAlloca(ctx->builder, LLVMInt32Type(), data->name);
    LLVMValueRef value = LLVMConstInt(LLVMInt32Type(), (unsigned long long int) data->value, 0);
    LLVMBuildStore(ctx->builder, value, alloca);

    g_hash_table_insert(ctx->vars, strdup(data->name), alloca);

}
