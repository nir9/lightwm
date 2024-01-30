#include <Windows.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h> 
#include "error.h"
#include "tiling.h"
#include "keyboard.h" 
#include "config.h"

#include "resource.h" 

#include "debug.h" 

HMODULE wmDll;
HHOOK hookShellProcHandle;
HANDLE windowEvent;

//Has to absolutely match the definition in the dll 
typedef LRESULT (*HotKeyProcType)(int, WPARAM, LPARAM);

void cleanupObjects() {
	if (hookShellProcHandle) {
		UnhookWindowsHookEx(hookShellProcHandle);
	}
	
	if (wmDll) {
		FreeLibrary(wmDll);
	}

	if (windowEvent) {
		CloseHandle(windowEvent);
	}
	
	cleanupConfigReader();
	cleanupKeyboard(); 
}

void ctrlc(int sig) {
	cleanupObjects();
	
	puts("Exiting"); 

	exit(ERROR_SUCCESS);
}

int main() {
	// Load Libraries and the needed functions from those libraries
	wmDll = LoadLibraryW(L"lightwm_dll");
	
	if (wmDll == NULL) {
		reportWin32Error(L"LoadLibrary of wm_dll"); 
		return ERROR_MOD_NOT_FOUND;
	}
	
	FARPROC shellProc = GetProcAddress(wmDll, "ShellProc");

	if (shellProc == NULL) { 
		reportWin32Error(L"GetProcAddress failed for shell even callback");
		goto cleanup; 
	}
	
	HotKeyProcType HotKeyProc = (HotKeyProcType)GetProcAddress(wmDll, "HotkeyProc");

	if (HotKeyProc == NULL) { 
		reportWin32Error(L"GetProcAddress failed for shell even callback");
		goto cleanup; 
	}
	
	windowEvent = CreateEventW(NULL, FALSE, FALSE, L"LightWMWindowEvent");

	if (windowEvent == NULL) {
		reportWin32Error(L"CreateEvent");
		goto cleanup;
	}

	hookShellProcHandle = SetWindowsHookExW(WH_SHELL, (HOOKPROC)shellProc, wmDll, 0);

	if (hookShellProcHandle == NULL) {
		reportWin32Error(L"SetWindowsHookExW failed for shell hook");
		goto cleanup;
	}
	
	signal(SIGINT, ctrlc);
 
	//Load the configuration
	if(loadConfigFile(NULL) != ERROR_SUCCESS) 
	{ 
		reportWin32Error(L"Load config file");
		goto cleanup; 
	}
	
	if(!initializeKeyboardConfig(getConfigItems())) 
	{ 
		reportWin32Error(L"Setup keyboard config"); 
		goto cleanup; 
	}
 
	// Handle a message loop
	tileWindows();
	MSG msg; 
	while (GetMessage(&msg, NULL, 0, 0) != 0) {
		if(msg.message == WM_HOTKEY) { 
			//Because Win32 doesn't support hook callbacks with RegisterHotkey lets make our own callback. 
			assert(HotKeyProc != NULL); 
			
			LRESULT ret = HotKeyProc(0, msg.wParam, msg.lParam);
			if(ret != ERROR_SUCCESS) { 
				DEBUG_PRINT("HotKey was unhandled! Ret: %i", ret); 
			}
			continue;
		} else if (WaitForSingleObject(windowEvent, INFINITE) == WAIT_FAILED) {
			reportWin32Error(L"WaitForSingleObject");
			goto cleanup;
		}

		Sleep(100);

		tileWindows();
	}

cleanup:
	cleanupObjects();
	
	return EXIT_FAILURE;
}
