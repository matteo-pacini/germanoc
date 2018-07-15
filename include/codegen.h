
#ifndef CODEGEN_H
#define CODEGEN_H

#include <llvm-c/Core.h>

#include <stdio.h>
#include <glib.h>

struct CodegenContext {
    LLVMModuleRef module;
    LLVMValueRef printf_fn;
    LLVMValueRef main_fn;
    LLVMValueRef scanf_fn;
    LLVMValueRef printf_str_fmt;
    LLVMValueRef printf_int_fmt;
    LLVMValueRef scanf_read_int_fmt;
    LLVMBuilderRef builder;
    GHashTable *vars;
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
