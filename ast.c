#include "ast.h"

ASTExprRef ASTExprCreatePrintLiteral(const gchar *str) {

    g_assert(str != NULL);
    ASTExprRef expr = g_new0(ASTExpr, 1);
    expr->type = AST_EXPR_TYPE_PRINT_LITERAL;
    expr->data = strdup(str);
    return expr;

}

void ASTExprDelete(ASTExprRef expr) {

    if (expr) {
        if (expr->type == AST_EXPR_TYPE_PRINT_LITERAL && expr->data) {
            free(expr->data);
        }
        free(expr);
    }

}
