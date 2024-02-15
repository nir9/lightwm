#define LIGHTWM_DLL

#include <Windows.h>
#include <stdio.h>
#include "messages.h"
#include "debug.h"
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
	switch(ulReasonForCall) { 
		case DLL_PROCESS_ATTACH:
			if (!retrieveDwordFromSharedMemory(&lightwmMainThreadId)) {
				reportGeneralError(L"Error retrieving the thread id from shared memory");
			}

			break; 
		case DLL_THREAD_ATTACH: 
			break;
		case DLL_THREAD_DETACH: 
			break; 
		case DLL_PROCESS_DETACH: 
			break;
		default:
			break;
	}

	return TRUE; 
}

