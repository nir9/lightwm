#include <Windows.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "error.h"
#include "tiling.h"

HHOOK hookHandle;

void ctrlc(int sig) {
	if (!UnhookWindowsHookEx(hookHandle)) {
		wprintf(L"[!] Error while unhooking hookHandle.\nError: %d\n", GetLastError());
	}
	
	puts("Exiting"); 

	// TODO: More cleanup

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

	HANDLE windowEvent = CreateEventW(NULL, FALSE, FALSE, L"LightWMWindowEvent");

	if (windowEvent == NULL) {
		reportWin32Error(L"CreateEvent failed");
		goto cleanup;
	}

	hookHandle = SetWindowsHookExW(WH_SHELL, (HOOKPROC)shellProc, wmDll, 0);

	if (hookHandle == NULL) {
		reportWin32Error(L"SetWindowsHookExW failed");
		goto cleanup;
	}

	signal(SIGINT, ctrlc);

	for (;;) {
		if (WaitForSingleObject(windowEvent, INFINITE) == WAIT_FAILED) {
			reportWin32Error(L"WaitForSingleObject failed");
			goto cleanup;
		}

		tileWindows();
	}

	Sleep(INFINITE);

cleanup:
	if (hookHandle) {
		UnhookWindowsHookEx(hookHandle);
	}

	if (wmDll) {
		FreeLibrary(wmDll);
	}

	if (windowEvent) {
		CloseHandle(windowEvent);
	}

	return EXIT_FAILURE;
}
