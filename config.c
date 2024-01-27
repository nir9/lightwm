/**
 * Config Reader and Parser
 * This program will read the config for lightwm in the appdata directory
 * 
 * Demetry Romanowski
 * demetryromanowski@gmail.com
 **/ 
#include "config.h" 

#include <Windows.h> 
#include <Shlobj.h> 
#include <wchar.h> 
#include <stdio.h> 
#include <strsafe.h> 
#include <shlwapi.h> 
#include <assert.h> 

#include "error.h" 
#include "resource.h"

/**
 * Config reader global vars
 **/
PWSTR szConfigFilePath[MAX_PATH]; 
char* defaultConfigData = NULL; 

typedef struct _ConfigItem { 
	char* name;
	char* value;
} ConfigItem; 

typedef struct _ConfigItems { 
	ConfigItem* configItem; 
	size_t configItemsCount;
} ConfigItems; 

//Should probably create a meta structure that holds the total count for now just another global variable
ConfigItems configItems; 

/**
 * Private prototypes here
**/
BOOL CreateDefaultConfigFile(HINSTANCE);
BOOL LoadDefaultConfigResourceData(HINSTANCE);
BOOL WriteDefaultConfigDataToFile(); 

DWORD ReadConfigFile() 
{ 
	//Try to open the config file based on the path
	FILE* configFileHandle = _wfopen(*szConfigFilePath, L"r"); 
	
	if(configFileHandle == NULL) 
	{
		SetLastError(ERROR_INVALID_HANDLE); 
		reportWin32Error(L"Config file could not be opened"); 
		return ERROR_INVALID_HANDLE; 
	}
	
	char line[256]; //TODO must have a more clever way of getting a line length
	uint32_t index = 0; 
	size_t lineCount = 0; 
	
	configItems.configItem = (ConfigItem*)malloc(sizeof(ConfigItem)); 
	configItems.configItem[0].name = (char*)malloc(sizeof(char) * 256);
	configItems.configItem[0].value = (char*)malloc(sizeof(char) * 256);
	
	if(configItems.configItem == NULL) 
	{ 
		reportWin32Error(L"Unable to allocate memory"); 
		CleanupConfigReader(); 
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	
	while(fgets(line, sizeof(line), configFileHandle)) { 
		if(strlen(line) == 0)
			continue; 
		
	
		char* token = strtok(line, " "); 
		size_t tokenCount = 0;
		while(token != NULL) {
			switch(tokenCount) {
				case 0: 
					configItems.configItem[lineCount].name = (char*)malloc(strlen(token));
					strcpy(configItems.configItem[lineCount].name, token); 
				case 1: 
					configItems.configItem[lineCount].value = (char*)malloc(strlen(token)); 
					strcpy(configItems.configItem[lineCount].value, token); 
			}
			tokenCount++; 
			token = strtok(NULL, " "); 
		}
		
		//Grab a clean copy of the old address before doing a realloc in case of failure. 
		ConfigItem* ptrClean = configItems.configItem; 
		configItems.configItem = (ConfigItem*)realloc(configItems.configItem, sizeof(ConfigItem) * ((++lineCount) + 1)); 
		
		if(configItems.configItem == NULL)
		{
			assert(ptrClean != NULL);
			free(ptrClean); 
			reportWin32Error(L"Unable to reallocate memory"); 
			CleanupConfigReader();
			return ERROR_INVALID_BLOCK; //TODO This might not be the best error code to return here 
		}
	}
	
	configItems.configItemsCount = lineCount; 
		
	fclose(configFileHandle); 
	
	return ERROR_SUCCESS;
}

void GetConfigFilePath() 
{ 
	//TODO: We don't check other results possible results, i.e E_FAIL
	HRESULT getAppDataPathResult = SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, NULL, szConfigFilePath); 
	
	if(!SUCCEEDED(getAppDataPathResult)) { 
		SetLastError(ERROR_PATH_NOT_FOUND); //SHGetKnownFolderPath does not set an error on fail so we set it manually
		reportWin32Error(L"Could not get the users appdata directory"); 
		CoTaskMemFree(szConfigFilePath); 
		exit(ERROR_PATH_NOT_FOUND);
	}
	
	HRESULT concatStringResult = StringCchCatW(*szConfigFilePath, MAX_PATH, L"\\lightwm.config");
	
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
		CoTaskMemFree(szConfigFilePath); 
		exit(GetLastError()); 
	}
}

uint8_t LoadConfigFile(HINSTANCE resourceModuleHandle) 
{ 
	GetConfigFilePath();

	if(!PathFileExistsW(*szConfigFilePath))
	{
		if(!CreateDefaultConfigFile(resourceModuleHandle)) 
		{ 
			SetLastError(ERROR_RESOURCE_NOT_AVAILABLE); 
			reportWin32Error(L"Create a default config file"); 
			return ERROR_RESOURCE_NOT_AVAILABLE; //TODO Maybe find a better error code here
		}
	}
	
	ReadConfigFile();

	return ERROR_SUCCESS; 
}

void CleanupConfigReader() 
{ 
	CoTaskMemFree(szConfigFilePath); 
	
	if(configItems.configItem) //TODO Cleanup the name and value pointers for the strings
	{
		for(size_t i = 0; i < configItems.configItemsCount; i++) 
		{
			if(&configItems.configItem[i] != NULL) 
			{ 
				if(configItems.configItem[i].name) 
				{ 
					free(configItems.configItem[i].name); 
				}
				
				if(configItems.configItem[i].value)
				{
					free(configItems.configItem[i].value); 
				}
			}
		}
		free(configItems.configItem); 
	}
}

ConfigItems* GetConfigItems()
{
	return &configItems;
}

/**
 * Private definitions here
 **/
BOOL CreateDefaultConfigFile(HINSTANCE resourceModuleHandle) 
{
	if(!LoadDefaultConfigResourceData(resourceModuleHandle)) 
		return FALSE;
	
	if(!WriteDefaultConfigDataToFile())
		return FALSE; 
	
	return TRUE; 
}

BOOL LoadDefaultConfigResourceData(HINSTANCE resourceModuleHandle)
{ 
	
	HRSRC hRes = FindResource(resourceModuleHandle, MAKEINTRESOURCE(IDR_DEFAULT_CONFIG), RT_RCDATA); 
	
	if(hRes == NULL) 
	{
		puts("Could not get HRSRC Handle"); 
		printf("%s %i\n", "FindResource Error: ", GetLastError());
		return FALSE; 
	}
	
	HGLOBAL hData = LoadResource(resourceModuleHandle, hRes); 
	
	if(hData == NULL) 
	{
		puts("Could not load resource"); 
		printf("%s %i\n", "LoadResource Error: ", GetLastError());
		return FALSE; 
	}
	
	LPVOID defaultConfigResourceData = LockResource(hData); 
	
	if(defaultConfigResourceData == NULL) 
	{
		puts("Could not read resource"); 
		printf("%s %i\n", "LockResource Error: ", GetLastError());
		return FALSE; 
	}
	
	size_t defaultConfigResourceDataLen = strlen(defaultConfigResourceData) + 1; //+1 for the null char
	defaultConfigData = (char*)malloc(sizeof(char) * defaultConfigResourceDataLen); //TODO Error checking
	strcpy(defaultConfigData, defaultConfigResourceData); 
	
	return TRUE; 
}

BOOL WriteDefaultConfigDataToFile() 
{ 
	FILE* configFileHandle = _wfopen(*szConfigFilePath, L"w"); 
	
	if(configFileHandle == NULL) 
	{ 
		return FALSE; 
	}

	fprintf(configFileHandle, defaultConfigData);

	puts("Created default config file"); 

	fclose(configFileHandle); 
	
	return TRUE;
}