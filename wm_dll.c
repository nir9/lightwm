#include <Windows.h>

#include "config_reader.h"


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
        switch (key->vkCode)
        {
        case VK_ESCAPE:
            puts("ESC pressed");
            break;
        case 'A':
            puts("A pressed");
            break;
        case VK_RETURN:
            puts("RETURN pressed");
            break;
        }
    }

    return CallNextHookEx(NULL, code, wparam, lparam);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ulReasonForCall, LPVOID lpReserved) 
{
	switch(ulReasonForCall) { 
		case DLL_PROCESS_ATTACH:
		{
			//----------------------------------------------
			/** 
			 * Read the config file here
			**/
			//----------------------------------------------
			LoadConfigFile(); 
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
		}
			break; 
	}

	return TRUE; 
}

