#include <Windows.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiling.h"
#include "error.h"
#include "keyboard.h"
#include "config.h"
#include "messages.h" 
#include "debug.h"

HMODULE wmDll;
HHOOK hookShellProcHandle;

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

LPVOID createAddressSharedMemory() {
	HANDLE hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(DWORD),
		"LightWMThreadId"
	);

	if (hMapFile == NULL) {
		DEBUG_PRINT("Could not create file mapping object (%lu).", GetLastError());
		return NULL;
	}

	LPVOID lpMapAddress = MapViewOfFile(
		hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(DWORD)
	);

	if (lpMapAddress == NULL) {
		DEBUG_PRINT("Could not map view of file (%lu).", GetLastError());
		CloseHandle(hMapFile);
		return NULL;
	}

	return lpMapAddress;
}

int main() {
    SetProcessDPIAware();

	if(!loadConfigFile(NULL))
	{ 
		reportWin32Error(L"Load config file");
		goto cleanup; 
	}
	
	if(!initializeKeyboardConfig(getConfigItems())) 
	{ 
		reportWin32Error(L"Setup keyboard config"); 
		goto cleanup; 
	}

	LPVOID sharedMemoryAddress = createAddressSharedMemory();

	if(sharedMemoryAddress == NULL) {
		reportWin32Error(L"Create Shared Memory");
		goto cleanup;
	}

	DWORD dwThreadId = GetCurrentThreadId();
	DEBUG_PRINT("Lightwm.exe thread id: %lu", dwThreadId);
	CopyMemory((PVOID)sharedMemoryAddress, &dwThreadId, sizeof(DWORD));

	wmDll = LoadLibraryW(L"lightwm_dll");
	
	if (wmDll == NULL) {
		reportWin32Error(L"LoadLibrary of lightwm_dll"); 
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

	signal(SIGINT, ctrlc);

	tileWindows();

	MSG msg;

	while (GetMessage(&msg, (HWND)-1, 0, 0) != 0) {
		switch (msg.message)
		{
			case WM_HOTKEY:
				const LRESULT ret = handleHotkey(msg.wParam, msg.lParam);

				if (ret != ERROR_SUCCESS) { 
					DEBUG_PRINT("HotKey was unhandled! Ret: %lli", ret); 
				}

				break; 
			case LWM_WINDOW_EVENT:
				tileWindows();
				DEBUG_PRINT("LWM_WINDOW_EVENT Message handled");
				break;
			default:
				break;
		}
	}

cleanup:
	cleanupObjects();
	
	return EXIT_FAILURE;
}
