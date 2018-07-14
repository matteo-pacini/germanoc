
#include "codegen.h"

#include <glib.h>

#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>

void _CodegenContextCodegenExpr(CodegenContextRef ctx, mpc_ast_t *node);
void _CodegenContextCodegenPrintExpr(CodegenContextRef ctx, mpc_ast_t *node);

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
    return ctx;
}

void CodegenContextDelete(CodegenContextRef ctx) {
    if (ctx) {
        if (ctx->builder) LLVMDisposeBuilder(ctx->builder);
        if (ctx->module) LLVMDisposeModule(ctx->module);
        g_free(ctx);
    }
}

void CodegenContextCodegen(CodegenContextRef ctx, mpc_val_t *ast) {

    g_assert(ctx != NULL);
    g_assert(ast != NULL);

    mpc_ast_trav_t *trav = mpc_ast_traverse_start(ast, mpc_ast_trav_order_pre);
    mpc_ast_t *current_node = mpc_ast_traverse_next(&trav);

    while (current_node != NULL) {
        if (g_str_has_prefix(current_node->tag, "expr")) {
            _CodegenContextCodegenExpr(ctx, current_node);
        }
        current_node = mpc_ast_traverse_next(&trav);
    }

    mpc_ast_traverse_free(&trav);

}

void _CodegenContextCodegenExpr(CodegenContextRef ctx, mpc_ast_t *node) {
    if (g_str_has_prefix(node->tag, "expr|print_expr")) {
        _CodegenContextCodegenPrintExpr(ctx, node);
    }
}

void _CodegenContextCodegenPrintExpr(CodegenContextRef ctx, mpc_ast_t *node) {

    gchar *quoted_string = node->children[node->children_num-1]->contents;
    gchar *clean_string = malloc(strlen(quoted_string-1) * sizeof(gchar));

    strncpy(clean_string, quoted_string+1, strlen(quoted_string)-2);
    clean_string[strlen(quoted_string)-2] = '\0';

    LLVMValueRef str = LLVMBuildGlobalStringPtr(
            ctx->builder,
            clean_string,
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

    free(clean_string);

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
    gchar *error;

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

    LLVMMemoryBufferRef buffer;
    LLVMTargetMachineEmitToMemoryBuffer(target_machine, ctx->module, LLVMAssemblyFile, &error, &buffer);

    LLVMDisposeTargetMachine(target_machine);

    fwrite(LLVMGetBufferStart(buffer), LLVMGetBufferSize(buffer), 1, file);
    fflush(file);

    LLVMDisposeMemoryBuffer(buffer);
}
