
#include "compiler.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#if _WIN32
    #include <windows.h>
    #include <tchar.h>
    #if defined(_WIN64)
        #define MINGW_CLANG_PATH "C:\\msys64\\mingw64\\bin\\clang.exe"
    #else
        #error "32 bit architecture is not supported"
    #endif
#elif __APPLE__
    #define CLANG_PATH "/usr/bin/clang"
    #include <TargetConditionals.h>
#elif __linux__
    #define CLANG_PATH "/usr/bin/clang"
    #include <sys/wait.h>
#endif


int _file_exists(const char *path) {
#if _WIN32
    // https://stackoverflow.com/a/6218957/2890168
    DWORD dwAttrib = GetFileAttributesA(path);
    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
            !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
#else
    // https://stackoverflow.com/a/230068/2890168
    return access(path, F_OK) != -1;
#endif
}

CompilerRef CompilerFind() {
// https://stackoverflow.com/a/5920028/2890168
#ifdef _WIN32
    if (_file_exists(MINGW_CLANG_PATH)) {
        CompilerRef compiler = malloc(sizeof(Compiler));
        compiler->type = CLANG;
        compiler->path = strdup(MINGW_CLANG_PATH);
        return compiler;
    }
    return NULL;
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
        #error "Compiler supports the macOS target only"
    #endif
#elif __linux__
        if (_file_exists(CLANG_PATH)) {
            CompilerRef compiler = malloc(sizeof(Compiler));
            compiler->type = CLANG;
            compiler->path = strdup(CLANG_PATH);
            return compiler;
        }
        return NULL;
#elif __unix__
        return NULL;
#elif defined(_POSIX_VERSION)
        return NULL;
#else
    #error "Unsupported operating system"
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

#if _WIN32

    const char *fmt = "%s %s -o %s";
    int cmdline_size = snprintf(NULL, 0, fmt, compiler->path, source, output);
    char *cmdline = malloc(sizeof(char) * (cmdline_size+1));
    snprintf(cmdline, (size_t) (cmdline_size + 1), fmt, compiler->path, source, output);

    // https://stackoverflow.com/a/22372528/2890168
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    LPTSTR szCmdline=_tcsdup(TEXT(cmdline));
    free(cmdline);

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    // Start the child process.
    if( !CreateProcess( NULL,   // No module name (use command line)
                        szCmdline,      // Command line
                        NULL,           // Process handle not inheritable
                        NULL,           // Thread handle not inheritable
                        TRUE,          // Set handle inheritance to FALSE
                        0,              // No creation flags
                        NULL,           // Use parent's environment block
                        NULL,           // Use parent's starting directory
                        &si,            // Pointer to STARTUPINFO structure
                        &pi )           // Pointer to PROCESS_INFORMATION structure
            )
    {
        fprintf(stderr, "CreateProcess failed (%d)./n", GetLastError() );
        return;
    }

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles.
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

#else

    char *args[] = {compiler->path, source, "-o", output,  NULL};

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

#endif
}
