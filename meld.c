#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <shellapi.h>
#include <shlwapi.h>

static wchar_t meld_bin[MAX_PATH + 10];

static wchar_t **get_argv()
{
    wchar_t **old_argv, **argv;
    int argc;
    int i;

    old_argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if(old_argv == NULL) {
        exit(1);
    }

    if (GetModuleFileNameW(NULL, meld_bin, MAX_PATH) == 0) {
        exit(1);
    }
    PathRemoveFileSpecW(meld_bin);
    wcscat(meld_bin, L"\\bin\\meld");

    argv = (wchar_t **) malloc(sizeof (*argv) * (argc + 2));
    argv[0] = old_argv[0];
    argv[1] = meld_bin;
    for(i = 1; i < argc; ++i) {
        argv[i + 1] = old_argv[i];
    }
    argv[argc + 1] = NULL;
    return argv;
}

static wchar_t **get_envp()
{
    int i, n;
    wchar_t *env_block, *p;
    wchar_t **envp;

    env_block = GetEnvironmentStringsW();
    if (env_block == NULL) {
        exit(1);
    }

    n = 0;
    p = env_block;
    while (*p != L'\0') {
        ++n;
        p += wcslen(p) + 1;
    }

    envp = (wchar_t **) malloc(sizeof (*envp) * n + 1);
    envp[0] = env_block;
    for (i = 1; i < n; ++i) {
        envp[i] = envp[i - 1] + wcslen(envp[i - 1]) + 1;
    }
    envp[n] = NULL;
    return envp;
}

#ifdef USE_WINMAIN
#define PYTHON_EXE L"pythonw.exe"
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
#define PYTHON_EXE L"python.exe"
int main(int _argc, char *_argv[])
#endif
{
    const wchar_t * const *argv;
    const wchar_t * const *envp;

    argv = (const wchar_t * const *) get_argv();
    _putenv("LANG=en_US");
    envp = (const wchar_t * const *) get_envp();
    _wexecvpe(PYTHON_EXE, argv, envp);
    return 0;
}
