#include <Windows.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

HHOOK hookHandle;

void ctrlc(int sig) {
	if (!UnhookWindowsHookEx(hookHandle)) {
		wprintf(L"[!] Error while unhooking hookHandle.\nError: %d\n", GetLastError());
	}
	
	puts("Exitting"); 
	exit(ERROR_SUCCESS);
}

int main() {
	HMODULE wmDll = LoadLibraryW(L"wm_dll");
	
	if (wmDll == NULL) {
	wprintf( L"[!] Error while loading library.\n Error: %ld", GetLastError());
		MessageBoxW(NULL, L"Could not load window managment library wm_dll.dll", L"Library Load Error", MB_OK | MB_ICONSTOP); 
		return ERROR_MOD_NOT_FOUND;
	}
	
	FARPROC shellProc = GetProcAddress(wmDll, "ShellProc");

	if (shellProc == NULL) { 
		DWORD err = GetLastError(); 
	wprintf(L"[!] Error while getting process address.\nError: %ld", err);
		
		wchar_t errorCodeBuff[sizeof(DWORD) * 8];
		swprintf(errorCodeBuff, sizeof(errorCodeBuff)/sizeof(wchar_t), L"%04lX", (int)err); 
		
		wchar_t dialogMsgBuff[100]; // Buffer large enough for the entire message
		swprintf(dialogMsgBuff, sizeof(dialogMsgBuff)/sizeof(wchar_t), L"Window management library seems to be corrupt. Error code: 0x%ls", errorCodeBuff);
		
		MessageBoxW(
			NULL, 
			dialogMsgBuff, 
			L"Error", 
			MB_OK | MB_ICONSTOP
		); 
	
		goto cleanup; 
	}

	hookHandle = SetWindowsHookExW(WH_SHELL, shellProc, wmDll, 0);

	if (hookHandle == NULL) {
		wprintf(L"[!] Error while getting hook.\nError: %ld", GetLastError());
		goto cleanup;
	}

	signal(SIGINT, ctrlc);

	Sleep(INFINITE);

	cleanup:
		if (wm_dll) {
			FreeLibrary(wm_dll);
		}

		if (hookHandle) {
			CloseHandle(hookHandle);
		}

		wprintf("[+] Finished Cleanup.\n");
	
	return EXIT_SUCCESS;
}
