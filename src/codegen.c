
#include "codegen.h"

#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Transforms/Scalar.h>

#include <mpc.h>

#include "ast.h"

inline LLVMTypeRef _printf_type();
inline LLVMTypeRef _main_type();

inline void _CodegenError(mpc_state_t *state, const char *fmt, ...);

void _CodegenContextCodegenExpr(CodegenContextRef ctx, ASTExprRef expr);
extern void _CodegenContextCodegenPrint(CodegenContextRef ctx, ASTExprRef expr);
extern void _CodegenContextCodegenPrintIdentifier(CodegenContextRef ctx, ASTExprRef expr);
extern void _CodegenContextCodegenVarDecl(CodegenContextRef ctx, ASTExprRef expr);
extern void _CodegenContextCodegenReadInt(CodegenContextRef ctx, ASTExprRef expr);
extern void _CodegenContextCodegenVarBlock(CodegenContextRef ctx, ASTExprRef expr);

LLVMTypeRef _printf_type() {
    LLVMTypeRef args[] = {
            LLVMPointerType(LLVMInt8Type(), 0)
    };
    return LLVMFunctionType(
            LLVMInt32Type(),
            args,
            1, //n_args
            1  //has_varargs
    );
}

LLVMTypeRef _main_type() {
    LLVMTypeRef args[] = {};
    return LLVMFunctionType(
            LLVMVoidType(),
            args,
            0, //n_args
            0  //has_varargs
    );
}

void _CodegenError(mpc_state_t *state, const char *fmt, ...) {

    va_list ap;
    va_start(ap, fmt);

    gchar *head = g_strdup_printf("[ERROR][%ld:%ld] ",
        state->row+1,
        state->col+1);
    gchar *tail = g_strdup_vprintf(fmt, ap);

    va_end(ap);

    fprintf(stderr, "%s%s\n", head, tail);

    g_free(head);
    g_free(tail);

}

void LLVMInit() {

    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();

}

CodegenContextRef CodegenContextCreate() {

    CodegenContextRef ctx = g_new0(CodegenContext, 1);

    ctx->module = LLVMModuleCreateWithName("mosconilang");

    ctx->printf_fn = LLVMAddFunction(ctx->module, "printf", _printf_type());
    ctx->scanf_fn = LLVMAddFunction(ctx->module, "scanf", _printf_type());
    ctx->main_fn = LLVMAddFunction(ctx->module, "main", _main_type());

    LLVMBasicBlockRef entrypoint = LLVMAppendBasicBlock(ctx->main_fn, "entrypoint");

    ctx->builder = LLVMCreateBuilder();
    LLVMPositionBuilderAtEnd(ctx->builder, entrypoint);

    ctx->printf_str_fmt = LLVMBuildGlobalStringPtr(ctx->builder, "%s\n", "printf_str_fmt");
    ctx->printf_int_fmt = LLVMBuildGlobalStringPtr(ctx->builder, "%d\n", "printf_int_fmt");
    ctx->scanf_read_int_fmt = LLVMBuildGlobalStringPtr(ctx->builder, "%d", "scanf_read_int_fmt");

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
            _CodegenContextCodegenPrint(ctx, expr);
            break;
        case AST_EXPR_TYPE_VAR_DECL:
            _CodegenContextCodegenVarDecl(ctx, expr);
            break;
        case AST_EXPR_TYPE_PRINT_ID:
            _CodegenContextCodegenPrintIdentifier(ctx, expr);
            break;
        case AST_EXPR_TYPE_READ_INT:
            _CodegenContextCodegenReadInt(ctx, expr);
            break;
        case AST_EXPR_TYPE_VAR_EXPR:
            break;
        case AST_EXPR_TYPE_VAR_BLOCK_EXPR:
            _CodegenContextCodegenVarBlock(ctx, expr);
            break;
    }

}

void CodegenContextAddRet(CodegenContextRef ctx) {

    g_assert(ctx != NULL);

    LLVMBuildRetVoid(ctx->builder);

    LLVMPassManagerRef pass = LLVMCreatePassManager();

    LLVMAddConstantPropagationPass(pass);
    LLVMAddInstructionCombiningPass(pass);
    LLVMAddPromoteMemoryToRegisterPass(pass);
    LLVMAddGVNPass(pass);
    LLVMAddCFGSimplificationPass(pass);

    LLVMRunPassManager(pass, ctx->module);
    LLVMDisposePassManager(pass);

}

void CodegenContextOutputIR(CodegenContextRef ctx, FILE *file) {

    g_assert(ctx != NULL);
    g_assert(file != NULL);

    gchar *ir = LLVMPrintModuleToString(ctx->module);

    fwrite(ir, strlen(ir), 1, file);
    fflush(file);

    free(ir);
}

void CodegenContextOutputASM(CodegenContextRef ctx, FILE *file, int verbose) {

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

    if (verbose) {
        printf("\nDefault target triple: %s\n", triple);
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

    LLVMMemoryBufferRef buffer;
    LLVMTargetMachineEmitToMemoryBuffer(target_machine, ctx->module, LLVMAssemblyFile, &error, &buffer);

    LLVMDisposeTargetMachine(target_machine);

    fwrite(LLVMGetBufferStart(buffer), LLVMGetBufferSize(buffer), 1, file);
    fflush(file);

    LLVMDisposeMemoryBuffer(buffer);
}
