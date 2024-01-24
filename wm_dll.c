#include <Windows.h>

HWND managed[256];
int currentManagedIndex = 0;

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lparam) {
	if (!IsWindowVisible(hwnd) || IsHungAppWindow(hwnd)) {
		return TRUE;
	}

	if (currentManagedIndex > 255) {
		return FALSE;
	}

	if (GetWindowTextLengthW(hwnd) == 0) {
		return TRUE;
	}

	RECT clientRect;
	if (!GetClientRect(hwnd, &clientRect)) {
		return TRUE;
	}

	// Skip small windows to avoid bugs
	if (clientRect.right < 100 || clientRect.bottom < 100){
		return TRUE;
	}

	managed[currentManagedIndex] = hwnd;
	currentManagedIndex++;
	return TRUE;
}

__declspec(dllexport) LRESULT CALLBACK ShellProc(int code, WPARAM wparam, LPARAM lparam) {

	if (code == HSHELL_WINDOWCREATED || code == HSHELL_WINDOWDESTROYED) {
		currentManagedIndex = 0;

		EnumChildWindows(GetDesktopWindow(), EnumChildProc, 0);

		if (currentManagedIndex != 0) {
			TileWindows(GetDesktopWindow(), MDITILE_VERTICAL | MDITILE_SKIPDISABLED, NULL, currentManagedIndex, managed);
		}
	}

	return CallNextHookEx(NULL, code, wparam, lparam);
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ulReasonForCall, LPVOID lpReserved) 
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
	}
		
	return TRUE; 
}

