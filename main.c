#include <assert.h>
#include <stdio.h>

#include "codegen.h"
#include "parser.h"
#include "str_utils.h"

#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>

int main(int argc, char *argv[]) {

    printf("MosconiLang JIT v" VERSION ".\n");
    str_print_authors(AUTHORS);

    ///////////////////////
    // Parser combinator //
    ///////////////////////

    mpc_err_t *parser_error = NULL;
    parser_init(&parser_error);
    if (parser_error) {
        mpc_err_print(parser_error);
        mpc_err_delete(parser_error);
        return EXIT_FAILURE;
    }

    mpc_result_t r;

    if (!mpc_parse_contents(argv[1], program, &r)) {
        mpc_err_print(r.error);
        mpc_err_delete(r.error);
        parser_deinit();
        return EXIT_FAILURE;
    } else {
        printf("AST:\n");
        mpc_ast_print(r.output);
        printf("\n");
    }

    mpc_ast_t *program_ast = r.output;

    ////////////////////
    // LLVM - Codegen //
    ////////////////////

    LLVMModuleRef mod = LLVMModuleCreateWithName("mosconilang");

    LLVMValueRef main = NULL;
    codegen(mod, &main, program_ast);

    char *verify_error = NULL;
    LLVMVerifyModule(mod, LLVMAbortProcessAction, &verify_error);

    if (strlen(verify_error) > 0) {
        fprintf(stderr, "LLVMVerifyModule error: \"%s\".\n", verify_error);
        LLVMDisposeMessage(verify_error);
        return EXIT_FAILURE;
    }

    mpc_ast_delete(program_ast);

    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMLinkInMCJIT();

    LLVMExecutionEngineRef engine;

    LLVMLinkInInterpreter();

    char *engine_error = NULL;

    if (LLVMCreateExecutionEngineForModule(&engine, mod, &engine_error)) {
        fprintf(stderr, "LLVMCreateExecutionEngineForModule error: \"%s\".\n", engine_error);
        LLVMDisposeMessage(engine_error);
        return EXIT_FAILURE;
    }

    LLVMGenericValueRef main_args[] = {};
    printf("Output:\n");
    LLVMRunFunction(engine, main, 0, main_args);

    LLVMDisposeExecutionEngine(engine);

    return 0;
}
