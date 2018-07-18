
#include "compiler.h"

#include <stdio.h>

#if _WIN32
    #if defined(_WIN64)
        static gchar *SEARCH_PATHS[] = {
            "C:\\msys64\\mingw64\\bin\\clang.exe" // MinGW
        };
    #else
        #error "32 bit architecture is not supported"
    #endif
#elif __APPLE__
    #include <TargetConditionals.h>
    static gchar *SEARCH_PATHS[] = {
        "/usr/bin/clang"
    };
#elif __linux__
    static gchar *SEARCH_PATHS[] = {
        "/usr/bin/gcc"
    };
#endif


static int _FileExists(const gchar *path) {
    return g_file_test(path, G_FILE_TEST_EXISTS);
}

static CompilerType _CompilerTypeForPath(const gchar *path) {
    gchar *basename = g_path_get_basename(path);
    if (g_str_equal(basename, "gcc") || g_str_equal(basename, "gcc.exe")) {
        free(basename);
        return GCC;
    }
    if (g_str_equal(basename, "clang") || g_str_equal(basename, "clang.exe")) {
        free(basename);
        return CLANG;
    }
    free(basename);
    return UNDEFINED;
}

CompilerRef CompilerFind() {

    for (gchar** path = SEARCH_PATHS; *path; path++) {
        if (_FileExists(*path)) {
            CompilerRef compiler = g_new0(Compiler, 1);
            compiler->path = strdup(*path);
            compiler->type = _CompilerTypeForPath(*path);
            return compiler;
        }
    }
    return NULL;

}

void CompilerDelete(CompilerRef compiler) {
    if (compiler) {
        if (compiler->path) free(compiler->path);
        free(compiler);
    }
}

void CompilerCompile(CompilerRef compiler, gchar *source, gchar* output) {

    g_assert(compiler != NULL && compiler->path != NULL);
    g_assert(source != NULL);
    g_assert(output != NULL);

    gchar *compiler_stdout = NULL;
    GError *error = NULL;

    gchar *argv[] = {compiler->path, source, "-o", output, NULL};

    gboolean result =
    g_spawn_sync(
        NULL,
        argv,
        NULL,
        G_SPAWN_DEFAULT,
        NULL,
        NULL,
        &compiler_stdout,
        NULL,
        NULL,
        &error
    );

    printf("Compiler output:\n\"%s\"\n", compiler_stdout);
    free(compiler_stdout);

    if (!result) {
        fprintf(stderr, "%s\n", error->message);
        g_error_free(error);
    }

}
