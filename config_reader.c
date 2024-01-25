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
#include <strsafe.h> 
#include <shlwapi.h> 

#include "error.h" 

PWSTR szPath[MAX_PATH]; 

void GetConfigFilePath() { 
	//TODO: We don't check other results possible results, i.e E_FAIL
	HRESULT getAppDataPathResult = SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, NULL, szPath); 
	
	if(!SUCCEEDED(getAppDataPathResult)) { 
		SetLastError(ERROR_PATH_NOT_FOUND); //SHGetKnownFolderPath does not set an error on fail so we set it manually
		reportWin32Error(L"Could not get the users appdata directory"); 
		CoTaskMemFree(szPath); 
		exit(ERROR_PATH_NOT_FOUND);
	}
	
	HRESULT concatStringResult = StringCchCatW(*szPath, MAX_PATH, L"\\lightwm.config");
	
	if(!SUCCEEDED(concatStringResult)) { 
		switch(concatStringResult){ 
			case STRSAFE_E_INVALID_PARAMETER: 
				SetLastError(ERROR_INVALID_PARAMETER); 
				break; 
			case STRSAFE_E_INSUFFICIENT_BUFFER: 
				SetLastError(ERROR_INSUFFICIENT_BUFFER);
				break; 
		}
		reportWin32Error(L"Could not append file name to appdata path"); 
		CoTaskMemFree(szPath); 
		exit(GetLastError()); 
	}
}

// TODO Remove this
void PrintString(PWSTR str) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Failed to get standard output handle.\n");
        return;
    }

    DWORD written;
    WriteConsoleW(hStdOut, str, wcslen(str), &written, NULL);
}

uint8_t CreateDefaultConfigFile()
{
	GetConfigFilePath(); 
	
	
	
	return 0;
}

uint8_t LoadConfigFile() { 
	
	GetConfigFilePath();

	if(!PathFileExistsW(*szPath))
	{
		PrintString(L"Config file does not exist\n"); 
		SetLastError(ERROR_FILE_NOT_FOUND);
		return ERROR_FILE_NOT_FOUND;
	}
	
	FILE* configFileHandle = _wfopen(*szPath, L"r"); 
	
	if(configFileHandle == NULL) 
	{
		PrintString(L"Config file could not be opened"); 
		SetLastError(ERROR_INVALID_HANDLE); 
		return ERROR_INVALID_HANDLE; 
	}
	
	char line[256]; //TODO Dynamically set line width 
	while(fgets(line, sizeof(line), configFileHandle)) { 
		printf("%s", line); 
	}
	
	fclose(configFileHandle); 
	
	return ERROR_SUCCESS; 
}

void CleanupConfigReader() { 
	CoTaskMemFree(szPath); 
}