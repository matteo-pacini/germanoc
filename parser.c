
#include "parser.h"

#include <mpc.h>

ParserRef ParserCreate(mpc_err_t **error) {

    g_assert(error != NULL);

    ParserRef parser = g_new0(Parser, 1);

    mpc_parser_t * string = mpc_new("string");
    mpc_parser_t * print_expr = mpc_new("print_expr");
    mpc_parser_t * expr = mpc_new("expr");
    mpc_parser_t * mosconilang = mpc_new("mosconilang");

    *error =
    mpca_lang(MPCA_LANG_DEFAULT,
              "string      : /\"(\\\\.|[^\"])*\"/                                                                      ;\n"
              "print_expr  : \"METTI\" \"UN\" (\"A\"|\"O\")? <string>                                                  ;\n"
              "expr        : <print_expr>                                                                              ;\n"
              "mosconilang : /^/ \"AMICI IN ASCOLTO, UN CORDIALE BUONGIORNO\" <expr>* \"ANDIAMO DALLA SIGLA, DAI\" /$/ ;\n",
              string,
              print_expr,
              expr,
              mosconilang,
              NULL
    );

    if (*error) {
        return NULL;
    }

    parser->_parser = mosconilang;

    parser->_subparsers = g_array_new(FALSE, FALSE, sizeof(mpc_parser_t*));
    g_array_append_val(parser->_subparsers, expr);
    g_array_append_val(parser->_subparsers, print_expr);
    g_array_append_val(parser->_subparsers, string);

    return parser;

}

void ParserDelete(ParserRef parser) {
    if (parser) {
        if (parser->_parser) mpc_delete(parser->_parser);
        if (parser->_subparsers) {
            for(int i=0; i<parser->_subparsers->len; i++) {
                mpc_delete(g_array_index(parser->_subparsers, mpc_parser_t*, i));
            }
            g_array_free(parser->_subparsers, FALSE);
        }
        if (parser->output) mpc_ast_delete(parser->output);
        if (parser->error) mpc_err_delete(parser->error);
        g_free(parser);
    }
}

void ParserParseFile(ParserRef parser, const gchar *filename) {

    g_assert(parser != NULL);
    g_assert(filename != NULL && strlen(filename) > 0);

    mpc_result_t result;
    mpc_parse_contents(filename, parser->_parser, &result);

    if (result.output) {
        parser->output = result.output;
    } else {
        parser->error = result.error;
    }

}
