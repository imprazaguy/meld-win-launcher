#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <shellapi.h>

static wchar_t **get_argv()
{
    wchar_t **argv_;
    int argc;
    int i;

    argv_ = CommandLineToArgvW(GetCommandLineW(), &argc);
    if(argv_ == NULL) {
        exit(1);
    }

    wchar_t **argv = (wchar_t **) malloc(sizeof (*argv) * (argc + 2));
    argv[0] = argv_[0];
    argv[1] = L"bin\\meld";
    for(i = 1; i < argc; ++i) {
        argv[i + 1] = argv_[i];
    }
    argv[argc + 1] = NULL;
    return argv;
}

static wchar_t **get_envp()
{
    wchar_t *env_block = GetEnvironmentStringsW();
    if (env_block == NULL) {
        exit(1);
    }

    int n = 0;
    wchar_t *p = env_block;
    while (*p != L'\0') {
        ++n;
        p += wcslen(p) + 1;
    }

    wchar_t **envp = (wchar_t **) malloc(sizeof (*envp) * n + 1);
    int i;
    envp[0] = env_block;
    for (i = 1; i < n; ++i) {
        envp[i] = envp[i - 1] + wcslen(envp[i - 1]) + 1;
    }
    envp[n] = NULL;
    return envp;
}

#ifdef USE_WINMAIN
#define PYTHON_EXE L"pythonw.exe"
int WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
#define PYTHON_EXE L"python.exe"
int main(int _argc, char *_argv[])
#endif
{
    const wchar_t * const *argv = (const wchar_t * const *) get_argv();
    _putenv("LANG=en_US");
    const wchar_t * const *envp = (const wchar_t * const *) get_envp();
    _wexecvpe(PYTHON_EXE, argv, envp);
    return 0;
}
