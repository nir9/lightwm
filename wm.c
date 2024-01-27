#include <Windows.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "error.h"
#include "tiling.h"

//Hooks
HHOOK hookHandle;
HHOOK keyboardHookHandle; 

//Modules
HMODULE wmDll;

//Event handles
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
	//----------------------------------------------
	/** 
	 * Load Libraries and the needed functions from those libraries
	**/
	//----------------------------------------------
	wmDll = LoadLibraryW(L"wm_dll");
	
	if (wmDll == NULL) {
		reportWin32Error(L"LoadLibrary of wm_dll"); 
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

	windowEvent = CreateEventW(NULL, FALSE, FALSE, L"LightWMWindowEvent");

	if (windowEvent == NULL) {
		reportWin32Error(L"CreateEvent");
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
	 * Handle a message loop
	**/
	//----------------------------------------------
	tileWindows();
	MSG msg; 
	while (GetMessage(&msg, NULL, 0, 0) != 0) {
		if (WaitForSingleObject(windowEvent, INFINITE) == WAIT_FAILED) {
			reportWin32Error(L"WaitForSingleObject");
			goto cleanup;
		}

		Sleep(100);

		tileWindows();
		
		TranslateMessage(&msg); 
		DispatchMessageW(&msg); 
	}

	//----------------------------------------------
	/** 
	 * Cleanup and gracefully exit
	**/

	//----------------------------------------------
cleanup:
	cleanupObjects();
	
	

	return EXIT_FAILURE;
}
