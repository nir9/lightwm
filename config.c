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

#include "debug.h"

#define BUFF_SIZE 65536

PWSTR szConfigFilePath; 

char* defaultConfigData = NULL; 

ConfigItems* configItems; 

BOOL createDefaultConfigFile(HINSTANCE);
BOOL loadDefaultConfigResourceData(HINSTANCE);
BOOL writeDefaultConfigDataToFile(); 
void trim(char* str); 
void removeControlChars(char* str); 
size_t getLineCount(FILE* file);

void freeConfigItems(ConfigItems* items);

DWORD readConfigFile() 
{ 
	assert(szConfigFilePath != NULL);

	//Try to open the config file based on the path
	FILE* configFileHandle = _wfopen(szConfigFilePath, L"r"); 
	
	if(configFileHandle == NULL) 
	{
		reportWin32Error(L"Config file could not be opened"); 
		cleanupConfigReader();
		return ERROR_INVALID_HANDLE; 
	}
	
	char line[BUFF_SIZE]; //TODO must have a more clever way of getting a line length
	
	configItems = (ConfigItems*)malloc(sizeof(ConfigItems)); 
	
	configItems->configItem = (ConfigItem*)malloc(sizeof(ConfigItem) * getLineCount(configFileHandle) + 1); 
	configItems->configItemsCount = getLineCount(configFileHandle) + 1; 

	if(configItems->configItem == NULL) 
	{ 
		reportWin32Error(L"Allocation ConfigItem memory"); 
		cleanupConfigReader(); 
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	
	for(size_t lineCount = 0; fgets(line, sizeof(line), configFileHandle); lineCount++) 
	{ 
		if(strlen(line) == 0)
		{
			continue; 
		}
		
		//Get the first half of the line
		char* token = strtok(line, " "); 
		configItems->configItem[lineCount].name = (char*)malloc(strlen(token) + 1);
		strncpy(configItems->configItem[lineCount].name, token, strlen(token) + 1); 
		
		//Get the second half of the line
		token = strtok(NULL, " "); 
		removeControlChars(token);
		configItems->configItem[lineCount].value = (char*)malloc(strlen(token) + 1); 
		strncpy(configItems->configItem[lineCount].value, token, strlen(token) + 1);
		
		DEBUG_PRINT("Name: %s Value: %s Name LEN: %zu Value LEN: %zu Count: %zu", 
			configItems->configItem[lineCount].name, 
			configItems->configItem[lineCount].value,
			strlen(configItems->configItem[lineCount].name), 
			strlen(configItems->configItem[lineCount].value), 
			lineCount); 
	}
	
	
	fclose(configFileHandle); 
	
	return ERROR_SUCCESS;
}

void getConfigFilePath() 
{ 	
	//TODO: We don't check other results possible results, i.e E_FAIL
	HRESULT getAppDataPathResult = SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, NULL, &szConfigFilePath); 
		
	if(!SUCCEEDED(getAppDataPathResult)) { 
		reportWin32Error(L"Could not get the users appdata directory"); 
		cleanupConfigReader();
		exit(ERROR_PATH_NOT_FOUND);
	}
	
	assert(szConfigFilePath != NULL);
	
	HRESULT concatStringResult = StringCchCatW(szConfigFilePath, MAX_PATH, L"\\lightwm.config");
	
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
		cleanupConfigReader();
		exit(GetLastError()); 
	}	
}

uint8_t loadConfigFile(HINSTANCE resourceModuleHandle) 
{ 
	szConfigFilePath = (PWSTR)malloc(sizeof(WCHAR) * MAX_PATH); 
	getConfigFilePath();
	assert(szConfigFilePath != NULL);
	
	if(!PathFileExistsW(szConfigFilePath))
	{
		if(!createDefaultConfigFile(resourceModuleHandle)) 
		{ 
			reportWin32Error(L"Create a default config file"); 
			return ERROR_RESOURCE_NOT_AVAILABLE; //TODO Maybe find a better error code here
		}
	}
	
	readConfigFile();

	return ERROR_SUCCESS; 
}

void freeConfigItems(ConfigItems* items) {
	if(items != NULL) {
		for (size_t i = 0; i < items->configItemsCount; i++) {
			if(&items->configItem[i] != NULL)
			{
				free(items->configItem[i].name);
				free(items->configItem[i].value);
			}
		}
		if(items->configItem != NULL)
		{
			free(items->configItem);
		}
		free(items);
	}
}

void cleanupConfigReader() 
{ 	
	freeConfigItems(configItems); 
	DEBUG_PRINT("Cleaned up config items"); 
	
	CoTaskMemFree(szConfigFilePath); 
	DEBUG_PRINT("Cleaned up filePath ptr"); 
}

ConfigItems* getConfigItems()
{
	return configItems;
}

/**
 * Private definitions here
 **/
BOOL createDefaultConfigFile(HINSTANCE resourceModuleHandle) 
{
	if(!loadDefaultConfigResourceData(resourceModuleHandle)) 
	{
		return FALSE;
	}
	
	if(!writeDefaultConfigDataToFile())
	{
		return FALSE; 
	}
	
	return TRUE; 
}

BOOL loadDefaultConfigResourceData(HINSTANCE resourceModuleHandle)
{ 
	
	HRSRC hRes = FindResource(resourceModuleHandle, MAKEINTRESOURCE(IDR_DEFAULT_CONFIG), RT_RCDATA); 
	
	if(hRes == NULL) 
	{
		puts("Could not get HRSRC Handle"); 
		DEBUG_PRINT("FindResource Error: %i", GetLastError()); 
		return FALSE; 
	}
	
	HGLOBAL hData = LoadResource(resourceModuleHandle, hRes); 
	
	if(hData == NULL) 
	{
		puts("Could not load resource"); 
		DEBUG_PRINT("LoadResource Error: %i", GetLastError());
		return FALSE; 
	}
	
	LPVOID defaultConfigResourceData = LockResource(hData); 
	
	if(defaultConfigResourceData == NULL) 
	{
		puts("Could not read resource"); 
		DEBUG_PRINT("LockResource Error: ", GetLastError());
		return FALSE; 
	}
	
	size_t defaultConfigResourceDataLen = strlen(defaultConfigResourceData) + 1; //+1 for the null char
	defaultConfigData = (char*)malloc(sizeof(char) * defaultConfigResourceDataLen); //TODO Error checking
	strcpy(defaultConfigData, defaultConfigResourceData); 
	
	return TRUE; 
}

BOOL writeDefaultConfigDataToFile() 
{ 
	FILE* configFileHandle = _wfopen(szConfigFilePath, L"w"); 
	
	if(configFileHandle == NULL) 
	{ 
		return FALSE; 
	}

	fprintf(configFileHandle, defaultConfigData);

	puts("Created default config file"); 

	fclose(configFileHandle); 
	
	return TRUE;
}

void strip(char* str) 
{
    size_t len = strlen(str);
    char *start = str;
    char *end = str + len - 1;

    while (*start == ' ') 
	{
		start++;
	}
	
    while (*end == ' ') 
	{
		end--;
	}

    memmove(str, start, end - start + 1);
    str[end - start + 1] = '\0';
}

void removeControlChars(char* str) 
{
    int i, j = 0;
    char temp[1024]; // Assuming the maximum length of the string is 1024

    for (i = 0; str[i] != '\0'; ++i) 
	{
        if ((unsigned char)str[i] < 32) continue; // Skip control characters
        temp[j] = str[i];
        ++j;
    }

    temp[j] = '\0'; // Null terminate the new string
    strncpy(str, temp, sizeof(temp));
}

size_t getLineCount(FILE* file) 
{
	char buf[BUFF_SIZE];
    int counter = 0;
    for(;;)
    {
        size_t res = fread(buf, 1, BUFF_SIZE, file);
        if (ferror(file))
		{
			return -1;
		}

        int i;
        for(i = 0; i < res; i++)
		{
			if (buf[i] == '\n')
			{
                counter++;
			}
		}

        if (feof(file))
		{
			break;
		}
    }
	
	fseek(file, 0, SEEK_SET); 

    return counter;
}