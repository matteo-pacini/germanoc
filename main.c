#include "codegen.h"
#include "parser.h"

#include <compiler.h>
#include <argtable3.h>

struct arg_lit *verbose, *help;
struct arg_file *output, *file;
struct arg_end *end;

int main(int argc, char *argv[]) {

    LLVMInit();

    //////////////////
    // Command Line //
    //////////////////

    void *argtable[] = {
            help    = arg_litn(NULL, "help", 0, 1, "Display this help and exit"),
            verbose = arg_litn(NULL, "verbose", 0, 1, "Be verbose"),
            output  = arg_filen("o", "output", "outfile", 0, 1, "Output file"),
            file    = arg_filen(NULL, NULL, "<FILE>", 0, 1, "Input file"),
            end     = arg_end(20),
    };

    int nerrors;
    nerrors = arg_parse(argc,argv,argtable);

    int should_quit_after_header = 0;

    if (help->count > 0)
    {
        printf("Usage: germanoc");
        arg_print_syntax(stdout, argtable, "\n");
        printf("MosconiLang compiler.\n\n");
        arg_print_glossary(stdout, argtable, "  %-25s %s\n");
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return EXIT_SUCCESS;
    }

    if (nerrors > 0) {
        arg_print_errors(stderr, end, "germanoc");
        fprintf(stderr, "Try 'germanoc --help' for more information.\n");
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return EXIT_FAILURE;
    }

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

    ParserParseFile(parser, *file->filename);

    if (parser->error) {
        mpc_err_print(error);
        ParserDelete(parser);
        return EXIT_FAILURE;
    }

    if (verbose->count > 0) {
        printf("AST:\n");
        mpc_ast_print(parser->output);
        printf("\n");
    }

    ////////////////////
    // LLVM - Codegen //
    ////////////////////

    CodegenContextRef ctx = CodegenContextCreate();
    CodegenContextCodegen(ctx, parser->output);
    CodegenContextAddRet(ctx);

    if (verbose->count > 0) {
        printf("LLVM IR:\n");
        CodegenContextOutputIR(ctx, stdout);
        printf("\nASM:\n");
        CodegenContextOutputASM(ctx, stdout);
        printf("\n");
    }

    ParserDelete(parser);

    //////////////
    // Compiler //
    //////////////

    CompilerRef compiler = CompilerFind();

    if (compiler) {

        const gchar *basename = *file->basename;
        gchar *source = g_strdup_printf("%s.s", basename);

        FILE *source_f = fopen(source, "w");
        CodegenContextOutputASM(ctx, source_f);
        fclose(source_f);

        CompilerCompile(compiler, source, (char *) *output->filename);
        g_free(source);

        CompilerDelete(compiler);

    } else {

        fprintf(stderr, "Could not find a valid compiler on the current target!\n");
        return EXIT_FAILURE;
        
    }

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
    CodegenContextDelete(ctx);

    return EXIT_SUCCESS;
}
