#include "codegen.h"
#include "parser.h"

#include <compiler.h>

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

    ParserDelete(parser);

    //////////////
    // Compiler //
    //////////////

    CompilerRef compiler = CompilerFind();

    if (compiler) {

        gchar *basename = g_path_get_basename(argv[1]);
        gchar *source = g_strdup_printf("%s.s", basename);
        gchar *output = g_strdup_printf("%s.compiled", basename);
        g_free(basename);

        FILE *source_f = fopen(source, "w");
        CodegenContextOutputASM(ctx, source_f);
        fclose(source_f);

        CompilerCompile(compiler, source, output);
        g_free(source);
        g_free(output);

        CompilerDelete(compiler);

    } else {

        fprintf(stderr, "Could not find a valid compiler on the current target!\n");
        return EXIT_FAILURE;
        
    }

    CodegenContextDelete(ctx);

    return EXIT_SUCCESS;
}
