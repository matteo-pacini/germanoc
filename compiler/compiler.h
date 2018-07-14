
#ifndef COMPILER_H
#define COMPILER_H

typedef enum {
    CLANG,
    GCC,
    VISUAL_STUDIO
} CompilerType;

struct Compiler {
    CompilerType type;
    char *path;
};

typedef struct Compiler  Compiler;
typedef struct Compiler* CompilerRef;

CompilerRef CompilerFind();
void        CompilerDelete(CompilerRef compiler);
void        CompilerCompile(CompilerRef compiler, char *source, char* output);

#endif //COMPILER_H
