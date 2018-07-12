
#ifndef CODEGEN_H
#define CODEGEN_H

#include <llvm-c/Core.h>

#include "mpc.h"

void codegen(LLVMModuleRef module, LLVMValueRef *main, mpc_ast_t *program);

#endif //CODEGEN_H
