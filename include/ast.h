
#ifndef AST_H
#define AST_H

#include <glib.h>

#include <mpc.h>

#include "macros.h"

typedef enum {
    AST_EXPR_TYPE_PRINT_LITERAL,
    AST_EXPR_TYPE_PRINT_ID,
    AST_EXPR_TYPE_VAR_DECL,
    AST_EXPR_TYPE_READ_INT,
    AST_EXPR_TYPE_VAR_EXPR,
    AST_EXPR_TYPE_VAR_BLOCK_EXPR
} ASTExprType;

typedef enum {
    AST_VAR_EXPR_TYPE_ADD_VAR,
    AST_VAR_EXPR_TYPE_ADD_INT,
    AST_VAR_EXPR_TYPE_SUB_VAR,
    AST_VAR_EXPR_TYPE_SUB_INT
} ASTVarExprType;

struct ASTExpr{
    mpc_state_t *state;
    ASTExprType type;
    void *data;
};

struct ASTVarDecl {
    gchar *name;
    gint32 value;
};

struct ASTVarBlockExpr {
    gchar *identifier;
    GPtrArray *ops;
};

struct ASTVarExpr {
    gchar *identifier;
    ASTVarExprType type;
    gint32 value;
};

TYPEDEF_STRUCT(ASTExpr);
TYPEDEF_STRUCT(ASTVarDecl);
TYPEDEF_STRUCT(ASTVarBlockExpr);
TYPEDEF_STRUCT(ASTVarExpr);

ASTExprRef ASTExprCreatePrintLiteral(const gchar *str);
ASTExprRef ASTExprCreatePrintIdentifier(const gchar *identifier);
ASTExprRef ASTExprCreateVarDecl(const gchar *identifier, gint32 value);
ASTExprRef ASTExprCreateReadInt(const gchar *identifier);

ASTExprRef ASTExprCreateVarExpr(const gchar *identifier, ASTVarExprType type, gint32 value);
ASTExprRef ASTExprCreateVarBlockExpr(const gchar *identifier, GPtrArray *ops);

void       ASTExprDelete(ASTExprRef expr);

#endif //AST_H
