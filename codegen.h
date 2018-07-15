
#ifndef CODEGEN_H
#define CODEGEN_H

#include <llvm-c/Core.h>

#include <stdio.h>
#include <glib.h>

struct CodegenContext {
    LLVMModuleRef module;
    LLVMValueRef printf_fn;
    LLVMValueRef main_fn;
    LLVMValueRef printf_str_fmt;
    LLVMBuilderRef builder;
};

typedef struct CodegenContext  CodegenContext;
typedef struct CodegenContext* CodegenContextRef;

void              LLVMInit();

CodegenContextRef CodegenContextCreate();
void              CodegenContextDelete(CodegenContextRef ctx);
void              CodegenContextCodegen(CodegenContextRef ctx, GPtrArray *exprs);
void              CodegenContextAddRet(CodegenContextRef ctx);
void              CodegenContextOutputIR(CodegenContextRef ctx, FILE *file);
void              CodegenContextOutputASM(CodegenContextRef ctx, FILE *file);

#endif //CODEGEN_H
