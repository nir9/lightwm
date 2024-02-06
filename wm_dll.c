#include <Windows.h>
#include <stdio.h>
#include "messages.h"
#include "debug.h"

DWORD lightwmMainThreadId = 0;

void readAddress() {
	HANDLE hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		"LightWMThreadId"
	);

	if (hMapFile == NULL) {
		DEBUG_PRINT("Could not open file mapping object (%lu).", GetLastError());
		return;
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
		return;
	}

	CopyMemory(&lightwmMainThreadId, (PVOID)lpMapAddress, sizeof(DWORD));

	UnmapViewOfFile(lpMapAddress);
	CloseHandle(hMapFile);
}

__declspec(dllexport) LRESULT CALLBACK ShellProc(int code, WPARAM wparam, LPARAM lparam) {
	if (code == HSHELL_WINDOWCREATED || code == HSHELL_WINDOWDESTROYED) {
		PostThreadMessageW(lightwmMainThreadId, LWM_WINDOW_EVENT, 0, 0);
	}

	return CallNextHookEx(NULL, code, wparam, lparam);
}

BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD ulReasonForCall, LPVOID lpReserved) 
{
	switch(ulReasonForCall) { 
		case DLL_PROCESS_ATTACH:
			DEBUG_PRINT("DLL Loaded");
			readAddress();
			break; 
		case DLL_THREAD_ATTACH: 
			break;
		case DLL_THREAD_DETACH: 
			break; 
		case DLL_PROCESS_DETACH: 
			DEBUG_PRINT("DLL Freed");
			break;
		default:
			break;
	}

	return TRUE; 
}

