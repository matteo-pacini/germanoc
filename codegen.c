
#include "codegen.h"

#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Transforms/Scalar.h>

#include <mpc.h>

#include "ast.h"

void _CodegenContextCodegenExpr(CodegenContextRef ctx, ASTExprRef expr);
void _CodegenContextCodegenPrintExpr(CodegenContextRef ctx, ASTExprRef expr);
void _CodegenContextCodegenPrintIdentifier(CodegenContextRef ctx, ASTExprRef expr);
void _CodegenContextCodegenVarDecl(CodegenContextRef ctx, ASTExprRef expr);

LLVMTypeRef _printf_type() {
    LLVMTypeRef args[] = {
            LLVMPointerType(LLVMInt8Type(), 0)
    };
    return LLVMFunctionType(
            LLVMInt32Type(),
            args,
            1,
            1
    );
}

LLVMTypeRef _main_type() {
    LLVMTypeRef args[] = {};
    return LLVMFunctionType(
            LLVMVoidType(),
            args,
            0,
            0
    );
}

void LLVMInit() {

    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();

}

CodegenContextRef CodegenContextCreate() {

    CodegenContextRef ctx = g_new0(CodegenContext, 1);

    ctx->module = LLVMModuleCreateWithName("mosconilang");

    ctx->printf_fn = LLVMAddFunction(ctx->module, "printf", _printf_type());
    ctx->main_fn = LLVMAddFunction(ctx->module, "main", _main_type());

    LLVMBasicBlockRef entrypoint = LLVMAppendBasicBlock(ctx->main_fn, "entrypoint");

    ctx->builder = LLVMCreateBuilder();
    LLVMPositionBuilderAtEnd(ctx->builder, entrypoint);

    ctx->printf_str_fmt = LLVMBuildGlobalStringPtr(ctx->builder, "%s\n", "printf_str_fmt");
    ctx->printf_int_fmt = LLVMBuildGlobalStringPtr(ctx->builder, "%d\n", "printf_int_fmt");

    ctx->vars = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);

    return ctx;

}

void CodegenContextDelete(CodegenContextRef ctx) {

    if (ctx) {
        if (ctx->builder) LLVMDisposeBuilder(ctx->builder);
        if (ctx->module) LLVMDisposeModule(ctx->module);
        if (ctx->vars) {
            g_hash_table_remove_all(ctx->vars);
            g_hash_table_destroy(ctx->vars);
        };
        g_free(ctx);
    }

}

void CodegenContextCodegen(CodegenContextRef ctx, GPtrArray *exprs) {

    g_assert(ctx != NULL);
    g_assert(exprs != NULL);

    for (int i=0; i<exprs->len; i++) {
        ASTExpr *expr = g_ptr_array_index(exprs, i);
        _CodegenContextCodegenExpr(ctx, expr);
    }

}

void _CodegenContextCodegenExpr(CodegenContextRef ctx, ASTExprRef expr) {

    g_assert(ctx != NULL);
    g_assert(expr != NULL);

    switch (expr->type) {

        case AST_EXPR_TYPE_PRINT_LITERAL:
            _CodegenContextCodegenPrintExpr(ctx, expr);
            break;
        case AST_EXPR_TYPE_VAR_DECL:
            _CodegenContextCodegenVarDecl(ctx, expr);
            break;
        case AST_EXPR_TYPE_PRINT_ID:
            _CodegenContextCodegenPrintIdentifier(ctx, expr);
            break;
    }

}

void _CodegenContextCodegenPrintExpr(CodegenContextRef ctx, ASTExprRef expr) {

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
        fprintf(stderr, "[ERROR][%ld:%ld] Variable \"%s\" is not defined.\n",
                expr->state->row+1,
                expr->state->col+1,
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

void _CodegenContextCodegenVarDecl(CodegenContextRef ctx, ASTExprRef expr) {

    g_assert(ctx != NULL);
    g_assert(expr != NULL);

    ASTVarDeclRef data = expr->data;

    if (g_hash_table_lookup(ctx->vars, data->name)) {
        fprintf(stderr, "[ERROR][%ld:%ld] Variable \"%s\" is already defined.\n",
                expr->state->row+1,
                expr->state->col+1,
                data->name);
        exit(EXIT_FAILURE);
    }

    LLVMValueRef alloca = LLVMBuildAlloca(ctx->builder, LLVMInt32Type(), data->name);
    LLVMValueRef value = LLVMConstInt(LLVMInt32Type(), (unsigned long long int) data->value, 0);
    LLVMBuildStore(ctx->builder, value, alloca);

    g_hash_table_insert(ctx->vars, strdup(data->name), alloca);

}

void CodegenContextAddRet(CodegenContextRef ctx) {

    g_assert(ctx != NULL);
    LLVMBuildRetVoid(ctx->builder);

}

void CodegenContextOutputIR(CodegenContextRef ctx, FILE *file) {

    g_assert(ctx != NULL);
    g_assert(file != NULL);

    gchar *ir = LLVMPrintModuleToString(ctx->module);

    fwrite(ir, strlen(ir), 1, file);
    fflush(file);

    free(ir);
}

void CodegenContextOutputASM(CodegenContextRef ctx, FILE *file) {

    g_assert(ctx != NULL);
    g_assert(file != NULL);

    gchar *triple = LLVMGetDefaultTargetTriple();
    gchar *error ;

    LLVMTargetRef target;
    if (LLVMGetTargetFromTriple(triple, &target, &error)) {
        fprintf(stderr, "Could not get native target: %s.\n", error);
        LLVMDisposeMessage(error);
        LLVMDisposeMessage(triple);
        return;
    }

    LLVMDisposeMessage(triple);

    LLVMTargetMachineRef target_machine =
        LLVMCreateTargetMachine(
                target,
                triple,
                "",
                "",
                LLVMCodeGenLevelDefault,
                LLVMRelocPIC,
                LLVMCodeModelDefault
        );

    LLVMPassManagerRef pass = LLVMCreatePassManager();

    LLVMAddConstantPropagationPass(pass);
    LLVMAddInstructionCombiningPass(pass);
    LLVMAddPromoteMemoryToRegisterPass(pass);
    LLVMAddGVNPass(pass);
    LLVMAddCFGSimplificationPass(pass);

    LLVMRunPassManager(pass, ctx->module);
    LLVMDisposePassManager(pass);

    LLVMMemoryBufferRef buffer;
    LLVMTargetMachineEmitToMemoryBuffer(target_machine, ctx->module, LLVMAssemblyFile, &error, &buffer);

    LLVMDisposeTargetMachine(target_machine);

    fwrite(LLVMGetBufferStart(buffer), LLVMGetBufferSize(buffer), 1, file);
    fflush(file);

    LLVMDisposeMemoryBuffer(buffer);
}
