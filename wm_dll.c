#include <Windows.h>

#include "targetver.h"
#include "keyboard.h" 
#include "config.h"

__declspec(dllexport) LRESULT CALLBACK ShellProc(int code, WPARAM wparam, LPARAM lparam) {
	if (code == HSHELL_WINDOWCREATED || code == HSHELL_WINDOWDESTROYED) {
		HANDLE windowEvent = OpenEventW(EVENT_ALL_ACCESS, FALSE, L"LightWMWindowEvent");
		if (windowEvent) {
			SetEvent(windowEvent);
			CloseHandle(windowEvent);
		}
	}

	return CallNextHookEx(NULL, code, wparam, lparam);
}

__declspec(dllexport) LRESULT CALLBACK KeyProc(int code, WPARAM wparam, LPARAM lparam) { 
	PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lparam;
    
    if (wparam == WM_KEYDOWN && code == HC_ACTION)
    {
		//Just example here
        if(IsKeyboardAction(key->vkCode))
		{
			puts("Action performed");
		}       
    }

    return CallNextHookEx(NULL, code, wparam, lparam);
}

BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD ulReasonForCall, LPVOID lpReserved) 
{
	switch(ulReasonForCall) { 
		case DLL_PROCESS_ATTACH:
		{
			//----------------------------------------------
			/** 
			 * Read the config file here
			**/
			//----------------------------------------------
			if(LoadConfigFile(hModule) != ERROR_SUCCESS) 
			{ 
				exit(GetLastError());
			}
			
			
			//----------------------------------------------
			/** 
			 * Initialize the keyboard config and actions here
			**/
			//----------------------------------------------
			if(!InitializeKeyboardConfig(GetConfigItems())) 
			{ 
				exit(GetLastError()); 
			}
		}	
			break; 
		case DLL_THREAD_ATTACH: 
			break;
		case DLL_THREAD_DETACH: 
			break; 
		case DLL_PROCESS_DETACH: 
		{
			//----------------------------------------------
			/** 
			 * Cleanup the config reader memory here
			**/
			//----------------------------------------------
			CleanupConfigReader();

			//----------------------------------------------
			/** 
			 * Cleanup the config reader memory here
			**/
			//----------------------------------------------
			CleanupKeyboard(); 
		}
			break; 
	}

	return TRUE; 
}

