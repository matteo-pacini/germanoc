#include "ast.h"

ASTExprRef ASTExprCreatePrintLiteral(const gchar *str) {

    g_assert(str != NULL);

    ASTExprRef expr = g_new0(ASTExpr, 1);
    expr->type = AST_EXPR_TYPE_PRINT_LITERAL;
    expr->data = strdup(str);

    return expr;

}

ASTExprRef ASTExprCreatePrintIdentifier(const gchar *identifier) {

    g_assert(identifier != NULL);

    ASTExprRef expr = g_new0(ASTExpr, 1);

    expr->type = AST_EXPR_TYPE_PRINT_ID;
    expr->data = strdup(identifier);

    return expr;

}

ASTExprRef ASTExprCreateVarDecl(const gchar *identifier, gint32 value) {

    g_assert(identifier != NULL);

    ASTExprRef expr = g_new0(ASTExpr, 1);
    expr->type = AST_EXPR_TYPE_VAR_DECL;

    ASTVarDeclRef data = g_new(ASTVarDecl, 1);
    data->name = strdup(identifier);
    data->value = value;

    expr->data = data;

    return expr;
}

ASTExprRef ASTExprCreateReadInt(const gchar *identifier) {

    g_assert(identifier != NULL);

    ASTExprRef expr = g_new0(ASTExpr, 1);
    expr->type = AST_EXPR_TYPE_READ_INT;
    expr->data = strdup(identifier);

    return expr;

}

ASTExprRef ASTExprCreateVarExpr(const gchar *identifier, ASTVarExprType type, gint32 value) {

    g_assert(
        (type == AST_VAR_EXPR_TYPE_ADD_VAR && identifier != NULL) ||
        (type == AST_VAR_EXPR_TYPE_ADD_INT && identifier == NULL) ||
        (type == AST_VAR_EXPR_TYPE_SUB_VAR && identifier != NULL) ||
        (type == AST_VAR_EXPR_TYPE_SUB_INT && identifier == NULL)
    )
    ;

    ASTExprRef expr = g_new0(ASTExpr, 1);
    expr->type = AST_EXPR_TYPE_VAR_EXPR;

    ASTVarExprRef var_expr = g_new0(ASTVarExpr, 1);
    if (identifier) {
        var_expr->identifier = strdup(identifier);
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

    ASTVarBlockExprRef block = g_new0(ASTVarBlockExpr, 1);
    block->identifier = strdup(identifier);
    block->ops = ops;

    expr->data = block;

    return expr;

}

void ASTExprDelete(ASTExprRef expr) {

    if (expr) {
        if (expr->type == AST_EXPR_TYPE_PRINT_ID) {
            if (expr->data) {
                free(expr->data); // gchar*
            }
        }

        if (expr->type == AST_EXPR_TYPE_PRINT_LITERAL) {
            if (expr->data) {
                free(expr->data); // gchar*
            }
        }

        if (expr->type == AST_EXPR_TYPE_VAR_DECL) {
            if (expr->data) {
                ASTVarDeclRef data = expr->data;
                free(data->name);
                g_free(data);
            }
        }

        if (expr->type == AST_EXPR_TYPE_READ_INT) {
            if (expr->data) {
                free(expr->data); // gchar*
            }
        }

        if (expr->type == AST_EXPR_TYPE_VAR_EXPR) {
            if (expr->data) {
                ASTVarExprRef var_expr = expr->data;
                if (var_expr->identifier) free(var_expr->identifier);
                g_free(var_expr);
            }
        }

        if (expr->type == AST_EXPR_TYPE_VAR_BLOCK_EXPR) {
            if (expr->data) {
                ASTVarBlockExprRef block = expr->data;
                if (block->identifier) free(block->identifier);
                if (block->ops) g_ptr_array_free(block->ops, TRUE);
                g_free(block);
            }
        }

        if (expr->state)  {
            g_free(expr->state);
        }

        g_free(expr);

    }

}
