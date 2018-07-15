
#include "parser.h"

#include "ast.h"

/////////////////
// DestroyFunc //
/////////////////

static void _garray_mpc_parser_t_free(gpointer data) {
    mpc_undefine(data);
    mpc_delete(data);
}

static void _garray_astexpr_free(gpointer data) {
    ASTExprDelete(data);
}

//////////
// Fold //
//////////

static mpc_val_t *mpcf_ast_expr_garray(int n, mpc_val_t **xs) {
    GPtrArray *array = g_ptr_array_new_with_free_func(_garray_astexpr_free);
    for (int i=0; i<n; i++) {
        ASTExpr *current = xs[i];
        g_ptr_array_add(array, current);
    }
    return array;
}

static mpc_val_t *mpcf_var_decl(int n, mpc_val_t **xs) {
    gchar *identifier = xs[2];
    gint32 value = *((gint32*)xs[4]);
    return ASTExprCreateVarDecl(identifier, value);
}

///////////
// Apply //
///////////

static mpc_val_t *mpc_to_print_expr(mpc_val_t* input) {
    const gchar *value = input;
    if (*value == '"') {
        gchar *cleaned_input = g_malloc0(sizeof(gchar) * (strlen(input)-1));
        strncpy(cleaned_input, input+1, strlen(input)-2);
        free(input);
        return ASTExprCreatePrintLiteral(cleaned_input);
    }
    return ASTExprCreatePrintIdentifier(input);
}

static mpc_val_t *mpc_string_hack(mpc_val_t* input) {
    gchar *result = g_strdup_printf("\"%s\"", (char *) input);
    free(input);
    return result;
}

//////////
// Dtor //
//////////

static void mpc_dtor_garray(mpc_val_t *val) {
    g_ptr_array_free(val, FALSE);
}

ParserRef ParserCreate() {

    ParserRef parser = g_new0(Parser, 1);

    mpc_parser_t * string = mpc_new("string");
    mpc_parser_t * integer = mpc_new("integer");
    mpc_parser_t * identifier = mpc_new("identifier");
    mpc_parser_t * print_expr = mpc_new("print_expr");
    mpc_parser_t * var_decl_expr = mpc_new("var_decl_expr");
    mpc_parser_t * expr = mpc_new("expr");
    mpc_parser_t * mosconilang = mpc_new("mosconilang");

    // string: string_literal (built-in)
    mpc_define(string,
        mpc_apply(
             mpc_string_lit(),
             mpc_string_hack
        )
    );

    // integer: integer_literal (built-in)
    mpc_define(integer, mpc_int());

    // identifier: identifier (built-in)
    mpc_define(identifier, mpc_ident());

    // print_expr : "METTI UN" ("A"|"O") (<string>|<identifier>)
    mpc_define(print_expr,
        mpc_apply(
            mpc_and(
                3,
                mpcf_trd,
                mpc_string("METTI UN"),
                mpc_tok(mpc_maybe(mpc_or(2, mpc_char('A'), mpc_char('O')))),
                mpc_tok(mpc_or(2, string, identifier)),
                free,
                free
            ),
            mpc_to_print_expr
        )
    );

    // var_decl_expr : "MA COS'E' ST" ("A"|"O") <identifier> ? <integer>
    mpc_define(var_decl_expr,
        mpc_and(
            5,
            mpcf_var_decl,
            mpc_string("MA COS'E' ST"),
            mpc_tok(mpc_or(2, mpc_char('A'), mpc_char('O'))),
            mpc_tok(identifier),
            mpc_tok(mpc_char('?')),
            mpc_tok(integer),
            free,
            free,
            free,
            free
        )
    );

    // expr: <print_expr>
    mpc_define(expr,
        mpc_or(
            2,
            print_expr,
            var_decl_expr
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

    parser->_subparsers = g_ptr_array_new_with_free_func(_garray_mpc_parser_t_free);
    g_ptr_array_add(parser->_subparsers, expr);
    g_ptr_array_add(parser->_subparsers, print_expr);
    g_ptr_array_add(parser->_subparsers, string);
    g_ptr_array_add(parser->_subparsers, integer);
    g_ptr_array_add(parser->_subparsers, identifier);
    g_ptr_array_add(parser->_subparsers, var_decl_expr);

    return parser;

}

void ParserDelete(ParserRef parser) {
    if (parser) {
        if (parser->_parser) {
            mpc_undefine(parser->_parser);
            mpc_delete(parser->_parser);
        }
        if (parser->_subparsers) {
            g_ptr_array_free(parser->_subparsers, TRUE);
        }
        if (parser->output) {
            g_ptr_array_free(parser->output, TRUE);
        }
        if (parser->error) {
            mpc_err_delete(parser->error);
        }
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
