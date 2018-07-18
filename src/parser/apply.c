
#include <mpc.h>
#include <glib.h>

#include "parser.h"

mpc_val_t *mpca_string_to_gvariant(mpc_val_t *input) {

    GVariant *v = g_variant_new_string(input);
    free(input);
    return v;

}

mpc_val_t *mpca_gint32_to_gvariant(mpc_val_t *input) {

    GVariant *v = g_variant_new_int32(*((gint32*)input));
    free(input);
    return v;

}

mpc_val_t *mpca_string_to_gvariant_identifier(mpc_val_t *input) {

    GVariant *v = g_variant_new("(si)", input, G_VARIANT_STRING_TYPE_IDENTIFIER);
    free(input);
    return v;

}

mpc_val_t *mpca_string_to_gvariant_literal(mpc_val_t *input) {

    GVariant *v = g_variant_new("(si)", input, G_VARIANT_STRING_TYPE_LITERAL);
    free(input);
    return v;

}
