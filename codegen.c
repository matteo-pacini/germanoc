
#include "codegen.h"

#include <glib.h>

#include <llvm-c/Core.h>

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

CodegenContextRef CodegenContextCreate() {
    CodegenContextRef ctx = g_new0(CodegenContext, 1);
    ctx->module = LLVMModuleCreateWithName("mosconilang");
    ctx->printf_fn = LLVMAddFunction(ctx->module, "printf", _printf_type());
    ctx->main_fn = LLVMAddFunction(ctx->module, "main", _main_type());
    LLVMBasicBlockRef entrypoint = LLVMAppendBasicBlock(ctx->main_fn, "entrypoint");
    ctx->builder = LLVMCreateBuilder();
    LLVMPositionBuilderAtEnd(ctx->builder, entrypoint);
    return ctx;
}

void CodegenContextDelete(CodegenContextRef ctx) {
    if (ctx) {
        if (ctx->builder) LLVMDisposeBuilder(ctx->builder);
        if (ctx->module) LLVMDisposeModule(ctx->module);
        g_free(ctx);
    }
}

void CodegenContextAddRet(CodegenContextRef ctx) {
    g_assert(ctx != NULL);
    LLVMBuildRetVoid(ctx->builder);
}
