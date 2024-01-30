#include <stdio.h>
#include <Windows.h>

#include "targetver.h"
#include "actions.h"

#include "debug.h"

__declspec(dllexport) LRESULT CALLBACK ShellProc(int code, WPARAM wparam, LPARAM lparam) {
	if (code == HSHELL_WINDOWCREATED || code == HSHELL_WINDOWDESTROYED) {
		const HANDLE windowEvent = OpenEventW(EVENT_ALL_ACCESS, FALSE, L"LightWMWindowEvent");
		if (windowEvent) {
			SetEvent(windowEvent);
			CloseHandle(windowEvent);
		}
	}

	return CallNextHookEx(NULL, code, wparam, lparam);
}

__declspec(dllexport) LRESULT HotkeyProc(int code, WPARAM wparam, LPARAM lparam) {
	DEBUG_PRINT("HotkeyProc called - %i %i %i", code, wparam, lparam); 
	
	switch(wparam) 
	{ 
		//TODO Can either trigger an event like the ShellProc callback, or handle directly. 
		// one method to handle virtual desktops is using the IVirtualDesktopManager in ShObjIdl but 
		// that is only available for Window 10 1809 or later. 
		case WORKSPACE_1:
			puts("Switch to workspace 1");
			break;
		case WORKSPACE_2:
			puts("Switch to workspace 2"); 
			break;
		case WORKSPACE_3:
			puts("Switch to workspace 3"); 
			break;
		case WORKSPACE_4:
			puts("Switch to workspace 4"); 
			break;
		case WINDOW_UP: 
			puts("Highlight window above"); 
			break; 
		case WINDOW_DOWN: 
			puts("Highlight window below"); 
			break; 
		case WINDOW_LEFT: 
			puts("Highlight window left"); 
			break; 
		case WINDOW_RIGHT: 
			puts("Highlight window right"); 
			break; 
		default: 
			DEBUG_PRINT("Unhandled hotkey message! Hotkey ID: %i", wparam); 
			break; 
	}
	
	return CallNextHookEx(NULL, code, wparam, lparam);
}

BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD ulReasonForCall, LPVOID lpReserved) 
{
	switch(ulReasonForCall) { 
		case DLL_PROCESS_ATTACH:
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

