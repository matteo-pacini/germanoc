
#include "parser.h"

#include <mpc.h>

#include "ast.h"

//////////
// Fold //
//////////

static void _garray_astexpr_free(gpointer data) {
    ASTExprDelete(data);
}

static mpc_val_t *mpcf_ast_expr_garray(int n, mpc_val_t **xs) {
    GArray *array = g_array_new(FALSE, FALSE, sizeof(ASTExpr*));
    for (int i=0; i<n; i++) {
        ASTExpr *current = xs[i];
        g_array_append_val(array, current);
    }
    g_array_set_clear_func(array, _garray_astexpr_free);
    return array;
}

///////////
// Apply //
///////////

static mpc_val_t *mpc_to_print_expr(mpc_val_t* input) {
    return ASTExprCreatePrintLiteral(input);
}

//////////
// Dtor //
//////////

static void mpc_dtor_garray(mpc_val_t *val) {
    g_array_free(val, FALSE);
}

ParserRef ParserCreate() {

    ParserRef parser = g_new0(Parser, 1);

    mpc_parser_t * string = mpc_new("string");
    mpc_parser_t * print_expr = mpc_new("print_expr");
    mpc_parser_t * expr = mpc_new("expr");
    mpc_parser_t * mosconilang = mpc_new("mosconilang");

    // string: string_literal
    mpc_define(string,
        mpc_apply(
            mpc_string_lit(),
            mpcf_unescape
        )
    );

    // print_expr : "METTI UN" ("A"|"O") <string>
    mpc_define(print_expr,
        mpc_apply(
            mpc_and(
                3,
                mpcf_trd,
                mpc_string("METTI UN"),
                mpc_tok(mpc_maybe(mpc_or(2, mpc_char('A'), mpc_char('O')))),
                mpc_tok(string),
                free,
                free
            ),
            mpc_to_print_expr
        )
    );

    // expr: <print_expr>
    mpc_define(expr,
        mpc_or(
            1,
            print_expr
        )
    );

    // mosconilang: "AMICI IN ASCOLTO, UN CORDIALE BUONGIORNO" <expr>* "ANDIAMO DALLA SIGLA, DAI"
    mpc_define(mosconilang,
       mpc_strip(
           mpc_and(
               3,
               mpcf_snd,
               mpc_tok(mpc_string("AMICI IN ASCOLTO, UN CORDIALE BUONGIORNO")),
               mpc_many(
                   mpcf_ast_expr_garray,
                   expr
               ),
               mpc_string("ANDIAMO DALLA SIGLA, DAI"),
               free,
               mpc_dtor_garray
           )
       )
    );

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
        if (parser->output) g_array_free(parser->output, TRUE);
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
