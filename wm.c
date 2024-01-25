#include <Windows.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "error.h"
#include "tiling.h"

HHOOK hookHandle;
HMODULE wmDll;
HANDLE windowEvent;

void cleanupObjects() {
	if (hookHandle) {
		UnhookWindowsHookEx(hookHandle);
	}

	if (wmDll) {
		FreeLibrary(wmDll);
	}

	if (windowEvent) {
		CloseHandle(windowEvent);
	}
}

void ctrlc(int sig) {
	cleanupObjects();
	
	puts("Exiting"); 

	exit(ERROR_SUCCESS);
}

int main() {
	wmDll = LoadLibraryW(L"wm_dll");
	
	if (wmDll == NULL) {
		reportWin32Error(L"LoadLibrary of wm_dll"); 
		return ERROR_MOD_NOT_FOUND;
	}
	
	FARPROC shellProc = GetProcAddress(wmDll, "ShellProc");

	if (shellProc == NULL) { 
		reportWin32Error(L"GetProcAddress");
		goto cleanup; 
	}

	windowEvent = CreateEventW(NULL, FALSE, FALSE, L"LightWMWindowEvent");

	if (windowEvent == NULL) {
		reportWin32Error(L"CreateEvent");
		goto cleanup;
	}

	hookHandle = SetWindowsHookExW(WH_SHELL, (HOOKPROC)shellProc, wmDll, 0);

	if (hookHandle == NULL) {
		reportWin32Error(L"SetWindowsHookExW");
		goto cleanup;
	}

	signal(SIGINT, ctrlc);

	for (;;) {
		if (WaitForSingleObject(windowEvent, INFINITE) == WAIT_FAILED) {
			reportWin32Error(L"WaitForSingleObject");
			goto cleanup;
		}

		if (!tileWindows()) {
			goto cleanup;
		}
	}

cleanup:
	cleanupObjects();

	return EXIT_FAILURE;
}
