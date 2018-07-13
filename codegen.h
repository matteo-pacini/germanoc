
#ifndef CODEGEN_H
#define CODEGEN_H

#include <llvm-c/Core.h>

#include "mpc.h"

struct CodegenContext {
    LLVMModuleRef module;
    LLVMValueRef printf_fn;
    LLVMValueRef main_fn;
    LLVMValueRef printf_str_fmt;
    LLVMBuilderRef builder;
};

typedef struct CodegenContext  CodegenContext;
typedef struct CodegenContext *CodegenContextRef;

CodegenContextRef CodegenContextCreate();
void              CodegenContextDelete(CodegenContextRef ctx);
void              CodegenContextAddRet(CodegenContextRef ctx);

#endif //CODEGEN_H
