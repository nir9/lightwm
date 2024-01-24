#include <Windows.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "error.h"

HHOOK hookHandle;

void ctrlc(int sig) {
	if (!UnhookWindowsHookEx(hookHandle)) {
		wprintf(L"[!] Error while unhooking hookHandle.\nError: %d\n", GetLastError());
	}
	
	puts("Exiting"); 
	exit(ERROR_SUCCESS);
}

int main() {
	HMODULE wmDll = LoadLibraryW(L"wm_dll");
	
	if (wmDll == NULL) {
		reportWin32Error(L"LoadLibrary of wm_dll failed"); 
		return ERROR_MOD_NOT_FOUND;
	}
	
	FARPROC shellProc = GetProcAddress(wmDll, "ShellProc");

	if (shellProc == NULL) { 
		reportWin32Error(L"GetProcAddress failed");
		goto cleanup; 
	}

	hookHandle = SetWindowsHookExW(WH_SHELL, (HOOKPROC)shellProc, wmDll, 0);

	if (hookHandle == NULL) {
		reportWin32Error(L"SetWindowsHookExW failed");
		goto cleanup;
	}

	signal(SIGINT, ctrlc);

	Sleep(INFINITE);

cleanup:
	if (wmDll) {
		FreeLibrary(wmDll);
	}

	return EXIT_FAILURE;
}
