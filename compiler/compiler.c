
#include "compiler.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if __APPLE__
    #define CLANG_PATH "/usr/bin/clang"
    #include <TargetConditionals.h>
    #include <unistd.h>
#endif

int _file_exists(const char *path) {
#if _WIN32
    return 0;
#else
    //https://stackoverflow.com/a/230068/2890168
    return access(path, F_OK ) != -1;
#endif
}

char *_executable_name(CompilerType t) {
    switch (t) {
        case CLANG: return "./clang";
        case GCC: return "./gcc";
        case VISUAL_STUDIO: return "cl.exe";
    }
}

CompilerRef CompilerFind() {
// https://stackoverflow.com/a/5920028/2890168
#ifdef _WIN32
#elif __APPLE__
    #if TARGET_OS_MAC
        if (_file_exists(CLANG_PATH)) {
            CompilerRef compiler = malloc(sizeof(Compiler));
            compiler->type = CLANG;
            compiler->path = strdup(CLANG_PATH);
            return compiler;
        }
        return NULL;
    #else
        #error "Compiler only works on macOS!"
    #endif
#elif __linux__
#elif __unix__
#elif defined(_POSIX_VERSION)
    #error "Unsupported operating system!"
#endif
}

void CompilerDelete(CompilerRef compiler) {
    if (compiler) {
        if (compiler->path) free(compiler->path);
        free(compiler);
    }
}

void CompilerCompile(CompilerRef compiler, char *source, char* output) {

    assert(compiler != NULL);
    assert(source != NULL);
    assert(output != NULL);

    char *args[] = {_executable_name(compiler->type), source, "-o", output,  NULL};

    int status;
    pid_t parent_pid;
    pid_t child_pid;

    parent_pid = getpid();
    child_pid = fork();

    switch(child_pid) {
        case -1:
            fprintf(stderr, "Fork failed (-1).\n");
        case 0:
            execv(compiler->path, args);
            // Shouldn't get here
            fprintf(stderr, "Fork failed (0).\n");
        default:
            break;
    }

    wait(&status);

    // TODO Check status
}
