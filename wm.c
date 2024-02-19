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
#include "shared_mem.h"

HMODULE wmDll;
HHOOK hookShellProcHandle;

void cleanupObjects()
{
	cleanupKeyboard();
	
	cleanupConfigReader();

	if (hookShellProcHandle) {
		UnhookWindowsHookEx(hookShellProcHandle);
	}
	
	if (wmDll) {
		FreeLibrary(wmDll);
	}

	cleanupMemoryMapFile();
}

void ctrlc(int sig)
{
	cleanupObjects();
	exit(ERROR_SUCCESS);
}

int main()
{
    SetProcessDPIAware();

	if (!loadConfigFile(NULL)) {
		reportGeneralError(L"Load config file");
		goto cleanup; 
	}
	
	if (!initializeKeyboardConfig(getConfigItems())) {
		reportGeneralError(L"Setup keyboard config"); 
		goto cleanup; 
	}

	if (!storeDwordInSharedMemory(GetCurrentThreadId())) {
		reportGeneralError(L"Failed writing thread id to shared memory");
		goto cleanup;
	}

	wmDll = LoadLibraryW(L"lightwm_dll");
	
	if (wmDll == NULL) {
		reportWin32Error(L"LoadLibrary of lightwm_dll"); 
		goto cleanup;
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
		switch (msg.message) {
			case WM_HOTKEY:
				handleHotkey(msg.wParam, msg.lParam);
				break; 
			case LWM_WINDOW_EVENT:
				tileWindows();
				break;
			default:
				break;
		}
	}

cleanup:
	cleanupObjects();
	
	return EXIT_FAILURE;
}
