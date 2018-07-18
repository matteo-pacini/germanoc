
#include <mpc.h>
#include <glib.h>

#include "parser.h"
#include "ast.h"

extern void garray_astexpr_free(gpointer data);

mpc_val_t *mpcf_ast_expr_garray(int n, mpc_val_t **xs) {

    // Do not free the expressions here.
    // GPtrArray will be the new owner.

    GPtrArray *array = g_ptr_array_new_with_free_func(garray_astexpr_free);
    for (int i=0; i<n; i++) {
        g_ptr_array_add(array, xs[i]);
    }
    return array;
}

// var_decl_expr : "MA COS'E' ST" ("A"|"O") |STATE| <identifier> ? <integer>
mpc_val_t *mpcf_var_decl(int n, mpc_val_t **xs) {

    free(xs[0]);
    free(xs[1]);
    mpc_state_t* state = xs[2]; // will be copied
    gchar *identifier = xs[3];  // will be copied by ASTExprCreateVarDecl
    free(xs[4]);
    gint32 value = *((gint32*)xs[5]); free(xs[5]);

    ASTExprRef node = ASTExprCreateVarDecl(identifier, value);
    free(identifier);

    node->state = g_new0(mpc_state_t, 1);
    memcpy(node->state, state, sizeof(mpc_state_t));
    free(state);

    return node;

}

// read_int_expr : DAMME CAMPO LUNGO SU |STATE| <identifier>
mpc_val_t *mpcf_read_int(int n, mpc_val_t **xs) {

    free(xs[0]);
    mpc_state_t* state = xs[1]; // will be copied
    gchar *identifier = xs[2];  // will be copied by ASTExprCreateReadInt

    ASTExprRef node = ASTExprCreateReadInt(identifier);
    free(identifier);

    node->state = g_new0(mpc_state_t, 1);
    memcpy(node->state, state, sizeof(mpc_state_t));
    free(state);

    return node;

}

// print_expr : "METTI UN" ("A"|"O") |STATE| (<string>|<identifier>)
mpc_val_t *mpcf_print_expr(int n, mpc_val_t **xs) {

    free(xs[0]);
    free(xs[1]);
    mpc_state_t* state = xs[2]; // will be copied
    GVariant* v = xs[3];        // will be disposed with g_variant_unref

    gchar *value = NULL;
    GVariantStringType type;

    // Gotta love the tuple <3!
    g_variant_get(v, "(si)", &value, &type);
    g_variant_unref(v);

    ASTExprRef expr = NULL;

    if (type == G_VARIANT_STRING_TYPE_IDENTIFIER) {
        expr = ASTExprCreatePrintIdentifier(value);
    } else {
        expr = ASTExprCreatePrintLiteral(value);
    }
    g_free(value);

    expr->state = g_new0(mpc_state_t, 1);
    memcpy(expr->state, state, sizeof(mpc_state_t));
    free(state);

    return expr;

}

// var_expr_sum: DOMANI A ZURIGO SI SVOLGERANNO GLI ACCOPPIAMENTI CON |STATE| (<identifier>|<integer>)
mpc_val_t *mpcf_var_expr_add(int n, mpc_val_t **xs) {

    free(xs[0]);
    mpc_state_t* state = xs[1]; // will be copied
    GVariant *v = xs[2];        // will be disposed with g_variant_unref

    ASTExprRef expr = NULL;

    if (g_variant_is_of_type(v, G_VARIANT_TYPE_INT32)) {
        gint32 value = g_variant_get_int32(v);
        expr = ASTExprCreateVarExpr(NULL, AST_VAR_EXPR_TYPE_ADD_INT, value);
    } else {
        gsize value_size;
        const gchar *value = g_variant_get_string(v, &value_size); // no need to free this - see func docs
        expr = ASTExprCreateVarExpr(value, AST_VAR_EXPR_TYPE_ADD_VAR, 0);
    }
    g_variant_unref(v);

    expr->state = g_new0(mpc_state_t, 1);
    memcpy(expr->state, state, sizeof(mpc_state_t));
    free(state);

    return expr;

}

// var_expr_sub: L'ATLETA DI FONDO E' STATO PRECEDUTO DA |STATE| (<identifier>|<integer>)
mpc_val_t *mpcf_var_expr_sub(int n, mpc_val_t **xs) {

    free(xs[0]);
    mpc_state_t* state = xs[1]; // will be copied
    GVariant *v = xs[2];        // will be disposed with g_variant_unref

    ASTExprRef expr = NULL;

    if (g_variant_is_of_type(v, G_VARIANT_TYPE_INT32)) {
        gint32 value = g_variant_get_int32(v);
        expr = ASTExprCreateVarExpr(NULL, AST_VAR_EXPR_TYPE_SUB_INT, value);
    } else {
        gsize value_size;
        const gchar *value = g_variant_get_string(v, &value_size); // no need to free this - see func docs
        expr = ASTExprCreateVarExpr(value, AST_VAR_EXPR_TYPE_SUB_VAR, 0);
    }
    g_variant_unref(v);

    expr->state = g_new0(mpc_state_t, 1);
    memcpy(expr->state, state, sizeof(mpc_state_t));
    free(state);

    return expr;

}

// var_block_expr : ANDIAMO ALLE NOTIZIE SU |STATE| <identifier> <var_expr>* ME TOCCA RIFARE TUTTO DA CAPO
mpc_val_t *mpcf_var_block_expr(int n, mpc_val_t **xs) {

    free(xs[0]);
    mpc_state_t *state = xs[1]; // will be copied
    gchar *identifier = xs[2];  // will be copied
    GPtrArray *ops = xs[3];     // will be owned by ASTExprCreateVarBlockExpr
    free(xs[4]);

    //expr takes ownership of ops - do not free it here
    ASTExprRef expr = ASTExprCreateVarBlockExpr(identifier, ops);
    free(identifier);

    expr->state = g_new0(mpc_state_t, 1);
    memcpy(expr->state, state, sizeof(mpc_state_t));
    free(state);

    return expr;
}
