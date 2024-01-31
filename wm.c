#include <Windows.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h> 

#include "error.h"
#include "tiling.h"
#include "keyboard.h" 
#include "config.h"
#include "messages.h" 

#include "debug.h" 

HMODULE wmDll;
HHOOK hookShellProcHandle;

typedef BOOL (*SetMessageThreadIdType)(DWORD);

void cleanupObjects() {
	cleanupKeyboard();
	
	cleanupConfigReader();

	if (hookShellProcHandle) {
		UnhookWindowsHookEx(hookShellProcHandle);
	}
	
	if (wmDll) {
		FreeLibrary(wmDll);
	}
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
		reportWin32Error(L"GetProcAddress for ShellProc");
		goto cleanup; 
	}
	
	hookShellProcHandle = SetWindowsHookExW(WH_SHELL, (HOOKPROC)shellProc, wmDll, 0);

	if (hookShellProcHandle == NULL) {
		reportWin32Error(L"SetWindowsHookExW for shell hook");
		goto cleanup;
	}
	
	SetMessageThreadIdType setMessageThreadId = (SetMessageThreadIdType)GetProcAddress(wmDll, "SetMessageThreadId");

	if (setMessageThreadId == NULL) { 
		reportWin32Error(L"GetProcAddress for setMessageThreadId");
		goto cleanup; 
	}
	 
	setMessageThreadId(GetCurrentThreadId());
	
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
	//tileWindows();
	MSG msg; 
	while (GetMessage(&msg, -1, 0, 0) != 0) {
		switch(msg.message)
		{
			case WM_HOTKEY: 
				LRESULT ret = handleHotkey(msg.wParam, msg.lParam);
				if(ret != ERROR_SUCCESS) { 
					DEBUG_PRINT("HotKey was unhandled! Ret: %lli", ret); 
				}
				break; 
			case LWM_WINDOW_EVENT:
				//tileWindows();
				DEBUG_PRINT("LWM_WINDOW_EVENT Message handled");
				break; 
		}
	}

cleanup:
	cleanupObjects();
	
	return EXIT_FAILURE;
}
