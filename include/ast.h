
#ifndef AST_H
#define AST_H

#include <glib.h>

#include <mpc.h>

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
    AST_VAR_EXPR_TYPE_ADD_INT
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

struct ASTVarExpr {
    gchar *name;
    ASTVarExprType type;
    gint32 value;
};

typedef struct ASTExpr  ASTExpr;
typedef struct ASTExpr* ASTExprRef;

typedef struct ASTVarDecl  ASTVarDecl;
typedef struct ASTVarDecl* ASTVarDeclRef;

typedef struct ASTVarExpr  ASTVarExpr;
typedef struct ASTVarExpr* ASTVarExprRef;

ASTExprRef ASTExprCreatePrintLiteral(const gchar *str);
ASTExprRef ASTExprCreatePrintIdentifier(const gchar *ident);
ASTExprRef ASTExprCreateVarDecl(const gchar *name, gint32 value);
ASTExprRef ASTExprCreateReadInt(const gchar *name);

ASTExprRef ASTExprCreateVarExpr(const gchar *identifier, ASTVarExprType type, gint32 value);
ASTExprRef ASTExprCreateVarBlockExpr(const gchar *identifier, GPtrArray *ops);

void       ASTExprDelete(ASTExprRef expr);

#endif //AST_H
