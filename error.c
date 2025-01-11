#include <Windows.h>
#include "error.h"

void reportGeneralError(WCHAR* message)
{
	MessageBoxW(NULL, message, L"LightWM Error", MB_OK); 
}

void reportWin32Error(WCHAR* message)
{
	DWORD lastError = GetLastError();
	WCHAR* messageBuffer;

	reportGeneralError(message);
	if (FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		lastError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&messageBuffer,
		0,
		NULL) == 0) {
		reportGeneralError(L"FormatMessageW has failed");
		return;
	}

	MessageBoxW(NULL, messageBuffer, L"LightWM Last Win32 Error", MB_OK);

	LocalFree(messageBuffer);
}
