
#include "str_utils.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

char **_str_authors(const char *input, size_t *authors_n) {

    assert(input != NULL && authors_n != NULL);

    char *input_copy = strdup(input);

    *authors_n = 0;
    for (int i=0; i<strlen(input); i++) {
        char c = input[i];
        if (c == '|')  { *authors_n = *authors_n + 1; }
    }
    *authors_n = *authors_n + 1;

    char **authors = malloc(sizeof(char*) * (*authors_n));
    assert(authors != NULL);

    int i=0;
    char *token = strtok(input_copy, "|");
    authors[i] = strdup(token);
    i++;

    while ((token = strtok(NULL, "|")) != NULL) {
        authors[i] = strdup(token);
        i++;
    }

    free(input_copy);

    return authors;

}

void str_print_authors(const char *value) {

    size_t authors_n = 0;
    char **authors = _str_authors(value, &authors_n);

    for (int i=0; i<authors_n; i++) {
        printf("Author: %s.\n", authors[i]);
        free(authors[i]);
        if (i==authors_n-1) { printf("\n"); }
    }
    free(authors);

}
