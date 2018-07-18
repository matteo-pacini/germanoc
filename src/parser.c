
#include "parser.h"

#include "ast.h"

#include <sys/stat.h>

/////////////////
// DestroyFunc //
/////////////////

void garray_mpc_parser_t_free(gpointer data) {
    // as seen in mpc_cleanup(int n, ...) in mpc.c
    mpc_undefine(data);
    mpc_delete(data);
}

void garray_astexpr_free(gpointer data) {
    ASTExprDelete(data);
}

//////////
// Fold //
//////////

extern mpc_val_t *mpcf_ast_expr_garray(int n, mpc_val_t **xs);
extern mpc_val_t *mpcf_var_decl(int n, mpc_val_t **xs);
extern mpc_val_t *mpcf_read_int(int n, mpc_val_t **xs);
extern mpc_val_t *mpcf_print_expr(int n, mpc_val_t **xs);
extern mpc_val_t *mpcf_var_expr_add(int n, mpc_val_t **xs);
extern mpc_val_t *mpcf_var_expr_sub(int n, mpc_val_t **xs);
extern mpc_val_t *mpcf_var_block_expr(int n, mpc_val_t **xs);

///////////
// Apply //
///////////

extern mpc_val_t *mpca_string_to_gvariant(mpc_val_t *input);
extern mpc_val_t *mpca_gint32_to_gvariant(mpc_val_t *input);
extern mpc_val_t *mpca_string_to_gvariant_identifier(mpc_val_t *input);
extern mpc_val_t *mpca_string_to_gvariant_literal(mpc_val_t *input);

//////////
// Dtor //
//////////

static void mpc_dtor_garray(mpc_val_t *val) {
    g_ptr_array_free(val, TRUE);
}

////////////
// Parser //
////////////

static gchar *_ParserFilterComments(const gchar* buffer);

ParserRef ParserCreate() {

    ParserRef parser = g_new0(Parser, 1);

    mpc_parser_t * string = mpc_new("string");
    mpc_parser_t * integer = mpc_new("integer");
    mpc_parser_t * identifier = mpc_new("identifier");
    mpc_parser_t * print_expr = mpc_new("print_expr");
    mpc_parser_t * var_decl_expr = mpc_new("var_decl_expr");
    mpc_parser_t * read_int_expr = mpc_new("read_int_expr");
    mpc_parser_t * var_block_expr = mpc_new("var_block_expr");
    mpc_parser_t * var_expr = mpc_new("var_expr");
    mpc_parser_t * var_expr_add = mpc_new("var_expr_add");
    mpc_parser_t * var_expr_sub = mpc_new("var_expr_sub");
    mpc_parser_t * expr = mpc_new("expr");
    mpc_parser_t * mosconilang = mpc_new("mosconilang");

    // string: string_literal (built-in)
    mpc_define(string,mpc_string_lit());

    // integer: integer_literal (built-in)
    mpc_define(integer, mpc_int());

    // identifier: identifier (built-in)
    mpc_define(identifier, mpc_ident());

    // print_expr : "METTI UN" ("A"|"O") (<string>|<identifier>)
    mpc_define(print_expr,
        mpc_and(
            4,
            mpcf_print_expr,
            mpc_string("METTI UN"),
            mpc_tok(mpc_maybe(mpc_or(2, mpc_char('A'), mpc_char('O')))),
            mpc_state(),
            mpc_tok(
                mpc_or(
                    2,
                    mpc_apply(string, mpca_string_to_gvariant_literal),
                    mpc_apply(identifier, mpca_string_to_gvariant_identifier)
                )
            ),
            free,
            free,
            free
        )

    );

    // var_decl_expr : "MA COS'E' ST" ("A"|"O") <identifier> ? <integer>
    mpc_define(var_decl_expr,
        mpc_and(
            6,
            mpcf_var_decl,
            mpc_string("MA COS'E' ST"),
            mpc_tok(mpc_or(2, mpc_char('A'), mpc_char('O'))),
            mpc_state(),
            mpc_tok(identifier),
            mpc_tok(mpc_char('?')),
            mpc_tok(integer),
            free,
            free,
            free,
            free,
            free
        )
    );

    // read_int_expr : DAMME CAMPO LUNGO SU <identifier>
    mpc_define(read_int_expr,
        mpc_and(
           3,
           mpcf_read_int,
           mpc_tok(mpc_string("DAMME CAMPO LUNGO SU")),
           mpc_state(),
           mpc_tok(identifier),
           free,
           free
        )
    );

    // var_block_expr : ANDIAMO ALLE NOTIZIE SU <identifier> <var_expr>* ME TOCCA RIFARE TUTTO DA CAPO
    mpc_define(var_block_expr,
        mpc_and(
            5,
            mpcf_var_block_expr,
            mpc_tok(mpc_string("ANDIAMO ALLE NOTIZIE SU")),
            mpc_state(),
            mpc_tok(identifier),
            mpc_many(
                mpcf_ast_expr_garray,
                var_expr
            ),
            mpc_tok(mpc_string("ME TOCCA RIFARE TUTTO DA CAPO")),
            free,
            free,
            free,
            free
        )
    );

    // var_expr :
    mpc_define(var_expr,
        mpc_or(
            2,
            var_expr_add,
            var_expr_sub
        )
    );

    // var_expr_add: DOMANI A ZURIGO SI SVOLGERANNO GLI ACCOPPIAMENTI CON (<identifier>|<integer>)
    mpc_define(var_expr_add,
        mpc_and(
            3,
            mpcf_var_expr_add,
            mpc_tok(mpc_string("DOMANI A ZURIGO SI SVOLGERANNO GLI ACCOPPIAMENTI CON")),
            mpc_state(),
            mpc_tok(
                mpc_or(
                    2,
                    mpc_apply(identifier, mpca_string_to_gvariant),
                    mpc_apply(integer, mpca_gint32_to_gvariant)
                )
            ),
            free,
            free
        )
    );

    // var_expr_sub: L'ATLETA DI FONDO E' STATO PRECEDUTO DA (<identifier>|<integer>)
    mpc_define(var_expr_sub,
        mpc_and(
            3,
            mpcf_var_expr_sub, // same as ar_expr_add
            mpc_tok(mpc_string("L'ATLETA DI FONDO E' STATO PRECEDUTO DA")),
            mpc_state(),
            mpc_tok(
                mpc_or(
                    2,
                    mpc_apply(identifier, mpca_string_to_gvariant),
                    mpc_apply(integer, mpca_gint32_to_gvariant)
                )
            ),
            free,
            free
        )
    );

    // expr: <print_expr>    |
    //       <var_decl_expr> |
    //       <read_int_expr> |
    //       <var_block_expr>
    mpc_define(expr,
        mpc_or(
            4,
            print_expr,
            var_decl_expr,
            read_int_expr,
            var_block_expr
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

    parser->_subparsers = g_ptr_array_new_with_free_func(garray_mpc_parser_t_free);
    g_ptr_array_add(parser->_subparsers, expr);
    g_ptr_array_add(parser->_subparsers, print_expr);
    g_ptr_array_add(parser->_subparsers, string);
    g_ptr_array_add(parser->_subparsers, integer);
    g_ptr_array_add(parser->_subparsers, identifier);
    g_ptr_array_add(parser->_subparsers, var_decl_expr);
    g_ptr_array_add(parser->_subparsers, read_int_expr);
    g_ptr_array_add(parser->_subparsers, var_block_expr);
    g_ptr_array_add(parser->_subparsers, var_expr);
    g_ptr_array_add(parser->_subparsers, var_expr_add);
    g_ptr_array_add(parser->_subparsers, var_expr_sub);

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

static gchar *_ParserFilterComments(const gchar* buffer) {

    g_assert(buffer != NULL);

    size_t new_buffer_size = 0;
    gchar *it = (gchar *) buffer;

    while (*it != '\0') {
        if (*it == '#') {
            while (*it != '\n' && *it != '\r' && *it != EOF) it++;
        } else {
            new_buffer_size++; it++;
        }
    }

    gchar *new_buffer = malloc(sizeof(gchar) * (new_buffer_size+1));
    it = (gchar *) buffer;
    int idx = 0;

    while (*it != '\0') {
        if (*it == '#') {
            while (*it != '\n' && *it != '\r' && *it != EOF) it++;
        } else {
            new_buffer[idx] = *it; idx++; it++;
        }
    }

    new_buffer[new_buffer_size] = '\0';

    return new_buffer;

}

void ParserParseFile(ParserRef parser, const gchar *filename) {

    g_assert(parser != NULL);
    g_assert(filename != NULL && strlen(filename) > 0);

    gchar *buffer = NULL;
    gsize buffer_size = 0;
    GError *error = NULL;

    g_file_get_contents(filename, &buffer, &buffer_size, &error);

    char *new_buffer = _ParserFilterComments(buffer);
    free(buffer);

    mpc_result_t result;
    if (mpc_parse("input", new_buffer, parser->_parser, &result)) {
        parser->output = result.output;
    } else {
        parser->error = result.error;
    }

    free(new_buffer);

}
