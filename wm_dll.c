#include <Windows.h>
#include <stdio.h>

#include "targetver.h"
#include "messages.h"

#include "debug.h"

DWORD messageThreadId = 0; 

__declspec(dllexport) LRESULT CALLBACK ShellProc(int code, WPARAM wparam, LPARAM lparam) {
	DEBUG_PRINT("ShellProc hook called with a thread id of %i", messageThreadId); //TODO Cleanup this debug 
	if (code == HSHELL_WINDOWCREATED || code == HSHELL_WINDOWDESTROYED) {
		PostThreadMessageW(messageThreadId, LWM_WINDOW_EVENT, 0, 0);	
	}

	return CallNextHookEx(NULL, code, wparam, lparam);
}

__declspec(dllexport) void SetMessageThreadId(DWORD threadId) 
{
	DEBUG_PRINT("Set thread ID to %i", threadId); //TODO Cleanup this debug string
	messageThreadId = threadId; 
}

BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD ulReasonForCall, LPVOID lpReserved) 
{
	switch(ulReasonForCall) { 
		case DLL_PROCESS_ATTACH:
			DEBUG_PRINT("DLL Loaded"); 
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

