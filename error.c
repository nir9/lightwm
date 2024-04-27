#include <Windows.h>
#include "error.h"

void reportGeneralError(WCHAR* message)
{
	MessageBoxW(NULL, message, L"LightWM Error", MB_OK); 
}

void reportWin32Error(WCHAR* message)
{
	reportGeneralError(message);
}
