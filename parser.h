
#ifndef PARSER_H
#define PARSER_H

#include "mpc.h"

extern int parser_initialised;

extern mpc_parser_t *string;
extern mpc_parser_t *print_expr;
extern mpc_parser_t *expr;
extern mpc_parser_t *program;

void  parser_init(mpc_err_t **error);
void  parser_deinit();

#endif //PARSER_H
