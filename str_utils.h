
#ifndef STR_UTILS_H
#define STR_UTILS_H

#include <string.h>

// https://stackoverflow.com/a/4770992/2890168
#define STR_STARTS_WITH(PRE, STR) (strncmp(PRE, STR, strlen(PRE)) == 0)

void str_print_authors(const char *value);

#endif //STR_UTILS_H
