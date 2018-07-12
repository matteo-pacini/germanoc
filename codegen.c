
#include "codegen.h"

#include <assert.h>

inline LLVMTypeRef _main();
inline LLVMTypeRef _printf();

void codegen(LLVMModuleRef module, LLVMValueRef *main, mpc_ast_t *program) {

    assert(module != NULL);
    assert(main != NULL);
    assert(program != NULL);

    LLVMValueRef printf_func = LLVMAddFunction(module, "printf", _printf());
    *main = LLVMAddFunction(module, "main", _main());

    LLVMBasicBlockRef block = LLVMAppendBasicBlock(*main, "entrypoint");

    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMPositionBuilderAtEnd(builder, block);

    LLVMBuildRetVoid(builder);

    LLVMDisposeBuilder(builder);
}

LLVMTypeRef _main() {

    // void main(void)
    LLVMTypeRef func = LLVMFunctionType(
            LLVMVoidType(),
            NULL,
            0,
            0
    );

    return func;

}

LLVMTypeRef _printf() {

    // int printf(const char *format, ...)
    LLVMTypeRef args[] = { LLVMPointerType(LLVMInt8Type(), 0) };
    LLVMTypeRef func = LLVMFunctionType(
            LLVMInt32Type(),
            args,
            0,
            1
    );

    return func;

}
