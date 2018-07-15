#include "../include/ast.h"

ASTExprRef ASTExprCreatePrintLiteral(const gchar *str) {

    g_assert(str != NULL);
    ASTExprRef expr = g_new0(ASTExpr, 1);
    expr->type = AST_EXPR_TYPE_PRINT_LITERAL;
    expr->data = strdup(str);
    return expr;

}

ASTExprRef ASTExprCreatePrintIdentifier(const gchar *ident) {

    g_assert(ident != NULL);
    ASTExprRef expr = g_new0(ASTExpr, 1);
    expr->type = AST_EXPR_TYPE_PRINT_ID;
    expr->data = strdup(ident);
    return expr;

}

ASTExprRef ASTExprCreateVarDecl(const gchar *name, gint32 value) {

    g_assert(name != NULL);
    ASTExprRef expr = g_new0(ASTExpr, 1);
    expr->type = AST_EXPR_TYPE_VAR_DECL;
    ASTVarDeclRef data = g_new(ASTVarDecl, 1);
    data->name = strdup(name);
    data->value = value;
    expr->data = data;
    return expr;
}

void ASTExprDelete(ASTExprRef expr) {

    if (expr) {
        if (expr->type == AST_EXPR_TYPE_PRINT_ID) {
            if (expr->data) {
                free(expr->data);
            }
        } else {
            if (expr->type == AST_EXPR_TYPE_PRINT_LITERAL) {
                if (expr->data) {
                    free(expr->data);
                }
            }
        }
        if (expr->type == AST_EXPR_TYPE_VAR_DECL) {
            if (expr->data) {
                ASTVarDeclRef data = expr->data;
                free(data->name);
                free(data);
            }
        }
        if (expr->state) free(expr->state);
        free(expr);
    }

}
