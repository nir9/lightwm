#include <stdio.h>
#include <Windows.h>

#include "targetver.h"
#include "keyboard.h" 
#include "config.h"

#include "debug.h"


HWND hwnd;
const char g_szClassName[] = "HiddenWindowClass";

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

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_HOTKEY:
			DEBUG_PRINT("DEBUG wm_dll.c L26: wParam = %i\n", wParam);  
            switch (wParam) {
                case WORKSPACE_1:
					puts("Switch to workspace 1"); //TODO 
                    break;
                case WORKSPACE_2:
					puts("Switch to workspace 2"); //TODO 
                    break;
				case WORKSPACE_3:
					puts("Switch to workspace 3"); //TODO
					break; 
				case WORKSPACE_4:
					puts("Switch to workspace 4"); //TODO 
					break; 
				case WINDOW_UP:
					puts("Focus on window above current"); //TODO 
					break; 
				case WINDOW_DOWN:
					puts("Focus on window below current"); //TODO
					break;
				case WINDOW_LEFT:
					puts("Focus on window to the left of current"); //TODO 
					break;
				case WINDOW_RIGHT:
					puts("Focus on window to the right of current"); //TODO
					break;
            }
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
	
    return 0;
}

/**
 * This is terrible... but I cannot think of any other way to do it with the RegisterHotkey function
 */
BOOL CreateHiddenWindow(HINSTANCE hModule) 
{
	WNDCLASSEX wc;
	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.style         = 0;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hModule;
	wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, "Hidden Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 240, 120, NULL, NULL, hModule, NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}

	ShowWindow(hwnd, SW_HIDE);
	UpdateWindow(hwnd);
	
	return TRUE; 
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
				return FALSE; 
			}
			
			//----------------------------------------------
			/** 
			 * Create a hidden window to handle the hotkey messages
			**/
			//----------------------------------------------
			if(!CreateHiddenWindow(hModule))
			{
				return FALSE; 
			}
			
			//----------------------------------------------
			/** 
			 * Initialize the keyboard config and actions here
			**/
			//----------------------------------------------
			if(!InitializeKeyboardConfig(hwnd, GetConfigItems())) 
			{ 
				return FALSE; 
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
			
			//----------------------------------------------
			/** 
			 * Cleanup the hidden window
			**/
			//----------------------------------------------
			DestroyWindow(hwnd);
		}
			break;

		default:
			break;
	}

	return TRUE; 
}

