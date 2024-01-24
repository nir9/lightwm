#include <Windows.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "error.h"

HHOOK hookHandle;
HWND managed[256];
int currentManagedIndex = 0;

void ctrlc(int sig) {
	if (!UnhookWindowsHookEx(hookHandle)) {
		wprintf(L"[!] Error while unhooking hookHandle.\nError: %d\n", GetLastError());
	}
	
	puts("Exiting"); 

	// TODO: More cleanup

	exit(ERROR_SUCCESS);
}

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lparam) {
	if (!IsWindowVisible(hwnd) || IsHungAppWindow(hwnd)) {
		return TRUE;
	}

	if (currentManagedIndex > 255) {
		return FALSE;
	}

	if (GetWindowTextLengthW(hwnd) == 0) {
		return TRUE;
	}

	RECT clientRect;
	if (!GetClientRect(hwnd, &clientRect)) {
		return TRUE;
	}

	// Skip small windows to avoid bugs
	if (clientRect.right < 100 || clientRect.bottom < 100){
		return TRUE;
	}

	managed[currentManagedIndex] = hwnd;
	currentManagedIndex++;
	return TRUE;
}

void manage() {
	currentManagedIndex = 0;

	EnumChildWindows(GetDesktopWindow(), EnumChildProc, 0);

	if (currentManagedIndex != 0) {
		TileWindows(GetDesktopWindow(), MDITILE_VERTICAL | MDITILE_SKIPDISABLED, NULL, currentManagedIndex, managed);
	}
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
		WaitForSingleObject(windowEvent, INFINITE);
		manage();
	}

	Sleep(INFINITE);

cleanup:
	if (wmDll) {
		FreeLibrary(wmDll);
	}

	if (windowEvent) {
		CloseHandle(windowEvent);
	}

	return EXIT_FAILURE;
}
