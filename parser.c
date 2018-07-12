
#include "parser.h"

int parser_initialised = 0;

mpc_parser_t *string = NULL;
mpc_parser_t *print_expr = NULL;
mpc_parser_t *expr = NULL;
mpc_parser_t *program  = NULL;

void parser_init(mpc_err_t **error) {

    string = mpc_new("string");
    print_expr = mpc_new("print_expr");
    expr = mpc_new("expr");
    program = mpc_new("program");

    mpc_err_t * _error =
            mpca_lang(MPCA_LANG_DEFAULT,
                      "string     : /\"(\\\\.|[^\"])*\"/                                                                      ;\n"
                      "print_expr : \"METTI\" \"UN\" (\"A\")? <string>                                                        ;\n"
                      "expr       : <print_expr>                                                                              ;\n"
                      "program    : /^/ \"AMICI IN ASCOLTO, UN CORDIALE BUONGIORNO\" <expr>* \"ANDIAMO DALLA SIGLA, DAI\" /$/ ;\n",
                      string,
                      print_expr,
                      expr,
                      program,
                      NULL
            );

    if (_error && error) {
        *error = _error;
    }

    if (!_error) {
        parser_initialised = 1;
    }

}

void parser_deinit() {

    mpc_cleanup(4,
                string,
                print_expr,
                expr,
                program);

    parser_initialised = 0;

}
