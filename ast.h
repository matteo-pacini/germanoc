
#ifndef AST_H
#define AST_H

#include <glib.h>

typedef enum {
    AST_EXPR_TYPE_PRINT_LITERAL,
    AST_EXPR_TYPE_VAR_DECL
} ASTExprType;

struct ASTExpr{
    ASTExprType type;
    void *data;
};

struct ASTVarDecl {
    gchar *name;
    gint32 value;
};

typedef struct ASTExpr  ASTExpr;
typedef struct ASTExpr* ASTExprRef;

typedef struct ASTVarDecl  ASTVarDecl;
typedef struct ASTVarDecl* ASTVarDeclRef;

ASTExprRef ASTExprCreatePrintLiteral(const gchar *str);
ASTExprRef ASTExprCreateVarDecl(const gchar *name, gint32 value);
void       ASTExprDelete(ASTExprRef expr);

#endif //AST_H
