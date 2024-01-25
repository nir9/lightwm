/**
 * Config reader
 * This program will read the config for lightwm in the appdata directory
 * 
 * Demetry Romanowski
 * demetryromanowski@gmail.com
 **/ 
 
 
#include "config_reader.h" 

#include <Windows.h> 
#include <Shlobj.h> 
#include <wchar.h> 
#include <stdio.h> 

#include "error.h" 

void PrintString(PWSTR str) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Failed to get standard output handle.\n");
        return;
    }

    DWORD written;
    WriteConsoleW(hStdOut, str, wcslen(str), &written, NULL);
}

uint8_t LoadConfigFile() { 
	PWSTR szPath[MAX_PATH]; 
	
	//TODO: We don't check other results possible results, i.e E_FAIL
	HRESULT getAppDataPathResult = SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, NULL, szPath); 
	
	if(!SUCCEEDED(getAppDataPathResult)) { 
		SetLastError(ERROR_PATH_NOT_FOUND); //SHGetKnownFolderPath does not set an error on fail
		reportWin32Error(L"Could not get the users appdata directory"); 
		CoTaskMemFree(szPath); 
		exit(ERROR_PATH_NOT_FOUND);
	}

	PrintString(*szPath);
	
	CoTaskMemFree(szPath); 
	
	return 0; 
}