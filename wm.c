#include <Windows.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

HHOOK hookHandle;

void ctrlc(int sig) {
    if (!UnhookWindowsHookEx(hookHandle)) {
        wprintf(L"[!] Error while unhooking hookHandle.\nError: %d\n", GetLastError());
    }
    
    exit(0);
}

int main() {
    HMODULE wmDll = LoadLibraryW(L"wm_dll");

    if (wmDll == NULL) {
        wprintf( L"[!] Error while loading library.\n Error: %ld", GetLastError());
        return EXIT_FAILURE;
    }

    FARPROC shellProc = GetProcAddress(wmDll, "ShellProc");

    if (shellProc == NULL) {
        wprintf(L"[!] Error while getting process address.\nError: %ld", GetLastError());
        goto cleanup;
    }

    hookHandle = SetWindowsHookExW(WH_SHELL, shellProc, wmDll, 0);

    if (hookHandle == NULL) {
        wprintf(L"[!] Error while getting hook.\nError: %ld", GetLastError());
        goto cleanup;
    }

    signal(SIGINT, ctrlc);

    Sleep(INFINITE);

    cleanup:
        if (shellProc) {
            FreeLibrary(shellProc);
        }

        if (hookHandle) {
            CloseHandle(hookHandle);
        }

        wprintf("[+] Finished Cleanup.\n");
    
    return EXIT_SUCCESS;
}
