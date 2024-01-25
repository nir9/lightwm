#include <Windows.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "error.h"
#include "tiling.h"
#include "config_reader.h"

HHOOK hookHandle;
HHOOK keyboardHookHandle; 

void ctrlc(int sig) {
	if (!UnhookWindowsHookEx(hookHandle)) {
		wprintf(L"[!] Error while unhooking hookHandle.\nError: %d\n", GetLastError());
	}
	
	puts("Exiting"); 

	// TODO: More cleanup

	exit(ERROR_SUCCESS);
}

int main() {
	//----------------------------------------------
	/** 
	 * Load Libraries and the needed functions from those libraries
	**/
	//----------------------------------------------
	HMODULE wmDll = LoadLibraryW(L"wm_dll");
	
	if (wmDll == NULL) {
		reportWin32Error(L"LoadLibrary of wm_dll failed"); 
		return ERROR_MOD_NOT_FOUND;
	}
	
	FARPROC shellProc = GetProcAddress(wmDll, "ShellProc");

	if (shellProc == NULL) { 
		reportWin32Error(L"GetProcAddress failed for shell even callback");
		goto cleanup; 
	}
	
	FARPROC keyProc = GetProcAddress(wmDll, "KeyProc"); 
	
	if (keyProc == NULL) { 
		reportWin32Error(L"GetProcAddress failed for keyboard event callback"); 
		goto cleanup; 
	}

	HANDLE windowEvent = CreateEventW(NULL, FALSE, FALSE, L"LightWMWindowEvent");

	if (windowEvent == NULL) {
		reportWin32Error(L"CreateEvent failed");
		goto cleanup;
	}

	hookHandle = SetWindowsHookExW(WH_SHELL, (HOOKPROC)shellProc, wmDll, 0);

	if (hookHandle == NULL) {
		reportWin32Error(L"SetWindowsHookExW failed for shell hook");
		goto cleanup;
	}
	
	keyboardHookHandle = SetWindowsHookExW(WH_KEYBOARD_LL, (HOOKPROC)keyProc, wmDll, 0); 
	
	if (keyboardHookHandle == NULL) { 
		reportWin32Error(L"SetWindowsHookExW failed for keyboard hook"); 
		goto cleanup;
	}
	
	signal(SIGINT, ctrlc);

	//----------------------------------------------
	/** 
	 * Read the config file here
	**/
	//----------------------------------------------
	LoadConfigFile(); 
	

	//----------------------------------------------
	/** 
	 * Handle a message loop
	**/
	//----------------------------------------------
	MSG msg; 
	while (GetMessage(&msg, NULL, 0, 0) != 0) {
		if (WaitForSingleObject(windowEvent, INFINITE) == WAIT_FAILED) {
			reportWin32Error(L"WaitForSingleObject failed");
			goto cleanup;
		}

		tileWindows();
		
		TranslateMessage(&msg); 
		DispatchMessageW(&msg); 
	}

	Sleep(INFINITE);

	//----------------------------------------------
	/** 
	 * Cleanup and gracefully exit
	**/
	//----------------------------------------------
cleanup:
	if (hookHandle) {
		UnhookWindowsHookEx(hookHandle);
	}
	
	if (keyboardHookHandle) { 
		UnhookWindowsHookEx(keyboardHookHandle); 
	}

	if (wmDll) {
		FreeLibrary(wmDll);
	}

	if (windowEvent) {
		CloseHandle(windowEvent);
	}
	
	CleanupConfigReader(); 

	return EXIT_FAILURE;
}
