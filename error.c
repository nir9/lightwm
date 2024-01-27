#include <Windows.h>
#include <stdio.h>
#include "error.h"

void reportWin32Error(wchar_t* message) {
	DWORD err = GetLastError(); 

	wprintf(L"[!] Error: %s Failed.\nWin32 Last Error: %ld\n", message, err);
	
	wchar_t errorCodeBuff[sizeof(DWORD) * 8];

	swprintf(errorCodeBuff, sizeof(errorCodeBuff) / sizeof(wchar_t), L"%04lX", (int)err); 
	
	wchar_t dialogMsgBuff[100];

	swprintf(dialogMsgBuff, sizeof(dialogMsgBuff) / sizeof(wchar_t), L"Error: %s Failed. Error code: 0x%ls", message, errorCodeBuff);
	
	MessageBoxW(
		NULL, 
		dialogMsgBuff, 
		L"LightWM Error", 
		MB_OK | MB_ICONSTOP
	); 
}
