#include "codegen.h"
#include "parser.h"

int main(int argc, char *argv[]) {

    LLVMInit();

    ////////////
    // Header //
    ////////////

    printf("MosconiLang Compiler v" VERSION ".\n");
    gchar** authors = g_strsplit(AUTHORS, "|", 2);
    for (gchar **ptr = authors; *ptr; ptr++) {
        printf("Author: %s.\n", *ptr);
    }
    printf("\n");

    ///////////////////////
    // Parser combinator //
    ///////////////////////

    mpc_err_t *error;
    ParserRef parser = ParserCreate(&error);
    if (error) {
        mpc_err_print(error);
        mpc_err_delete(error);
        ParserDelete(parser);
        return EXIT_FAILURE;
    }

    ParserParseFile(parser, argv[1]);

    if (parser->error) {
        mpc_err_print(error);
        ParserDelete(parser);
        return EXIT_FAILURE;
    }

    printf("AST:\n");
    mpc_ast_print(parser->output);
    printf("\n");

    ////////////////////
    // LLVM - Codegen //
    ////////////////////

    CodegenContextRef ctx = CodegenContextCreate();
    CodegenContextCodegen(ctx, parser->output);
    CodegenContextAddRet(ctx);

    printf("LLVM IR:\n");
    CodegenContextOutputIR(ctx, stdout);
    printf("\nASM:\n");
    CodegenContextOutputASM(ctx, stdout);
    printf("\n");

    CodegenContextDelete(ctx);
    ParserDelete(parser);

    return 0;
}
