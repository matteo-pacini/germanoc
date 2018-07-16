#include "ast.h"

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

ASTExprRef ASTExprCreateReadInt(const gchar *name) {

    g_assert(name != NULL);
    ASTExprRef expr = g_new0(ASTExpr, 1);
    expr->type = AST_EXPR_TYPE_READ_INT;
    expr->data = strdup(name);
    return expr;

}

ASTExprRef ASTExprCreateVarExpr(const gchar *name, ASTVarExprType type, gint32 value) {

    ASTExprRef expr = g_new0(ASTExpr, 1);
    expr->type = AST_EXPR_TYPE_VAR_EXPR;

    ASTVarExprRef var_expr = g_new0(ASTVarExpr, 1);
    if (name) {
        var_expr->name = strdup(name);
    }
    var_expr->type = type;
    var_expr->value = value;

    expr->data = var_expr;

    return expr;

}

ASTExprRef ASTExprCreateVarBlockExpr(const gchar *identifier, GPtrArray *ops) {

    g_assert(identifier != NULL);
    g_assert(ops != NULL);

    ASTExprRef expr = g_new0(ASTExpr, 1);
    expr->type = AST_EXPR_TYPE_VAR_BLOCK_EXPR;
    expr->data = ops;

    return expr;

}

void ASTExprDelete(ASTExprRef expr) {

    if (expr) {
        if (expr->type == AST_EXPR_TYPE_PRINT_ID) {
            if (expr->data) {
                free(expr->data);
            }
        }

        if (expr->type == AST_EXPR_TYPE_PRINT_LITERAL) {
            if (expr->data) {
                free(expr->data);
            }
        }

        if (expr->type == AST_EXPR_TYPE_VAR_DECL) {
            if (expr->data) {
                ASTVarDeclRef data = expr->data;
                free(data->name);
                free(data);
            }
        }

        if (expr->type == AST_EXPR_TYPE_READ_INT) {
            if (expr->data) {
                free(expr->data);
            }
        }

        if (expr->type == AST_EXPR_TYPE_VAR_EXPR) {
            if (expr->data) {
                ASTVarExprRef var_expr = expr->data;
                if (var_expr->name) free(var_expr->name);
                free(var_expr);
            }
        }

        if (expr->type == AST_EXPR_TYPE_VAR_BLOCK_EXPR) {
            if (expr->data) {
                g_ptr_array_free(expr->data, TRUE);
            }
        }

        if (expr->state)  {
            free(expr->state);
        }

        free(expr);

    }

}
