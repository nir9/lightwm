// This DLL is loaded into many processes on the computer, therefore we need to keep the logic here as simple as possible to avoid slowing down the system
#define LIGHTWM_DLL

#include <Windows.h>
#include <stdio.h>
#include "messages.h"
#include "error.h"
#include "shared_mem.h"

DWORD lightwmMainThreadId = 0;

__declspec(dllexport) LRESULT CALLBACK ShellProc(int code, WPARAM wparam, LPARAM lparam) {
	if (code == HSHELL_WINDOWCREATED || code == HSHELL_WINDOWDESTROYED) {
		PostThreadMessageW(lightwmMainThreadId, LWM_WINDOW_EVENT, 0, 0);
	}

	return CallNextHookEx(NULL, code, wparam, lparam);
}

BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD ulReasonForCall, LPVOID lpReserved) 
{
	if (ulReasonForCall == DLL_PROCESS_ATTACH) { 
		if (!retrieveDwordFromSharedMemory(&lightwmMainThreadId)) {
			reportGeneralError(L"Error retrieving the thread id from shared memory");
		}
	}

	return TRUE; 
}
