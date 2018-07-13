#include "codegen.h"
#include "parser.h"

int main(int argc, char *argv[]) {

    ////////////
    // Header //
    ////////////

    printf("MosconiLang JIT v" VERSION ".\n");
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

    mpc_ast_print(parser->output);
    ParserDelete(parser);

    ////////////////////
    // LLVM - Codegen //
    ////////////////////

    CodegenContextRef ctx = CodegenContextCreate();
    CodegenContextAddRet(ctx);
    CodegenContextDelete(ctx);

    return 0;
}
