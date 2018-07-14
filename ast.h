
#ifndef AST_H
#define AST_H

#include <glib.h>

typedef enum {
    AST_EXPR_TYPE_PRINT_LITERAL
} ASTExprType;

struct ASTExpr{
    ASTExprType type;
    void *data;
};

typedef struct ASTExpr  ASTExpr;
typedef struct ASTExpr* ASTExprRef;

ASTExprRef ASTExprCreatePrintLiteral(const gchar *str);
void       ASTExprDelete(ASTExprRef expr);

#endif //AST_H
