
#include "ast.h"
#include "codegen.h"

extern void _CodegenError(mpc_state_t *state, const char *fmt, ...);

void _CodegenContextCodegenVarBlockAddVar(CodegenContextRef ctx, ASTExprRef expr);
void _CodegenContextCodegenVarBlockAddInt(CodegenContextRef ctx, ASTExprRef expr);
void _CodegenContextCodegenVarBlockSubVar(CodegenContextRef ctx, ASTExprRef expr);
void _CodegenContextCodegenVarBlockSubInt(CodegenContextRef ctx, ASTExprRef expr);

void _CodegenContextCodegenVarBlock(CodegenContextRef ctx, ASTExprRef expr) {

    g_assert(ctx != NULL);
    g_assert(expr != NULL);

    ASTVarBlockExprRef block = expr->data;
    LLVMValueRef alloca = g_hash_table_lookup(ctx->vars, block->identifier);

    if (!alloca) {
        _CodegenError(expr->state,
                      "Variable \"%s\" is not defined",
                      block->identifier);
        exit(EXIT_FAILURE);
    }

    ctx->current_var = alloca;

    for (int i=0; i<block->ops->len; i++) {
        ASTExprRef subexpr = g_ptr_array_index(block->ops, i);
        ASTVarExprRef op = subexpr->data;
        if (op->type == AST_VAR_EXPR_TYPE_ADD_VAR) {
            _CodegenContextCodegenVarBlockAddVar(ctx, subexpr);
        } else if (op->type == AST_VAR_EXPR_TYPE_ADD_INT) {
            _CodegenContextCodegenVarBlockAddInt(ctx, subexpr);
        } else if (op->type == AST_VAR_EXPR_TYPE_SUB_INT) {
            _CodegenContextCodegenVarBlockSubInt(ctx, subexpr);
        } else if (op->type == AST_VAR_EXPR_TYPE_SUB_VAR) {
            _CodegenContextCodegenVarBlockSubVar(ctx, subexpr);
        }
    }

    ctx->current_var = NULL;

}

void _CodegenContextCodegenVarBlockAddVar(CodegenContextRef ctx, ASTExprRef expr) {

    g_assert(ctx != NULL);
    g_assert(expr != NULL);

    ASTVarExprRef op = expr->data;

    LLVMValueRef rhs_alloca = g_hash_table_lookup(ctx->vars, op->identifier);

    if (!rhs_alloca) {
        _CodegenError(expr->state,
                      "Variable \"%s\" is not defined",
                      op->identifier);
        exit(EXIT_FAILURE);
    }

    LLVMValueRef load = LLVMBuildLoad(ctx->builder, ctx->current_var, "");
    LLVMValueRef rhs_load = LLVMBuildLoad(ctx->builder, rhs_alloca, "");
    LLVMValueRef add = LLVMBuildNSWAdd(ctx->builder, load, rhs_load, "");
    LLVMBuildStore(ctx->builder, add, ctx->current_var);

}

void _CodegenContextCodegenVarBlockAddInt(CodegenContextRef ctx, ASTExprRef expr) {

    g_assert(ctx != NULL);
    g_assert(expr != NULL);

    ASTVarExprRef op = expr->data;

    LLVMValueRef load = LLVMBuildLoad(ctx->builder, ctx->current_var, "");
    LLVMValueRef value = LLVMConstInt(LLVMInt32Type(), (unsigned long long int) op->value, 0);
    LLVMValueRef add = LLVMBuildNSWAdd(ctx->builder, load, value, "");
    LLVMBuildStore(ctx->builder, add, ctx->current_var);

}

void _CodegenContextCodegenVarBlockSubVar(CodegenContextRef ctx, ASTExprRef expr) {

    g_assert(ctx != NULL);
    g_assert(expr != NULL);

    ASTVarExprRef op = expr->data;

    LLVMValueRef rhs_alloca = g_hash_table_lookup(ctx->vars, op->identifier);

    if (!rhs_alloca) {
        _CodegenError(expr->state,
                      "Variable \"%s\" is not defined",
                      op->identifier);
        exit(EXIT_FAILURE);
    }

    LLVMValueRef load = LLVMBuildLoad(ctx->builder, ctx->current_var, "");
    LLVMValueRef rhs_load = LLVMBuildLoad(ctx->builder, rhs_alloca, "");
    LLVMValueRef sub = LLVMBuildNSWSub(ctx->builder, load, rhs_load, "");
    LLVMBuildStore(ctx->builder, sub, ctx->current_var);

}

void _CodegenContextCodegenVarBlockSubInt(CodegenContextRef ctx, ASTExprRef expr) {

    g_assert(ctx != NULL);
    g_assert(expr != NULL);

    ASTVarExprRef op = expr->data;

    LLVMValueRef load = LLVMBuildLoad(ctx->builder, ctx->current_var, "");
    LLVMValueRef value = LLVMConstInt(LLVMInt32Type(), (unsigned long long int) op->value, 0);
    LLVMValueRef sub = LLVMBuildNSWSub(ctx->builder, load, value, "");
    LLVMBuildStore(ctx->builder, sub, ctx->current_var);

}
