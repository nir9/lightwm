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
#include <assert.h> 

#include "error.h" 

PWSTR szPath[MAX_PATH]; 

typedef struct _ConfigItem { 
	char* name;
	char* value;
} ConfigItem; 

typedef struct _ConfigItems { 
	ConfigItem* ConfigItems; 
	size_t ConfigItemCount;
} ConfigItems; 

//Should probably create a meta structure that holds the total count for now just another global variable
ConfigItem* GPtrConfigItems = NULL; 
size_t GPtrConfigItemsCount = 0; 

DWORD ReadConfigFile() 
{ 
	FILE* configFileHandle = _wfopen(*szPath, L"r"); 
	
	if(configFileHandle == NULL) 
	{
		SetLastError(ERROR_INVALID_HANDLE); 
		reportWin32Error(L"Config file could not be opened"); 
		return ERROR_INVALID_HANDLE; 
	}
	
	char line[256]; //TODO must have a more clever way of getting a line length
	uint32_t index = 0; 
	size_t lineCount = 0; 
	
	GPtrConfigItems = (ConfigItem*)malloc(sizeof(ConfigItem)); 
	GPtrConfigItems[0].name = (char*)malloc(sizeof(char) * 256); // Need to free this memory when quitting
	GPtrConfigItems[0].value = (char*)malloc(sizeof(char) * 256);
	
	if(GPtrConfigItems == NULL) 
	{ 
		reportWin32Error(L"Unable to allocate memory"); 
		CleanupConfigReader(); 
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	
	while(fgets(line, sizeof(line), configFileHandle)) { 
		char* token = strtok(line, " "); 
		size_t tokenCount = 0;
		while(token != NULL) {
			switch(tokenCount) {
				case 0: 
					GPtrConfigItems[lineCount].name = (char*)malloc(sizeof(token));
					(void)strcpy(GPtrConfigItems[lineCount].name, token); 
				case 1: 
					GPtrConfigItems[lineCount].value = (char*)malloc(sizeof(token)); 
					(void)strcpy(GPtrConfigItems[lineCount].value, token); 
			}
			tokenCount++; 
			token = strtok(NULL, " "); 
		}
		
		//Grab a clean copy of the old address before doing a realloc in case of failure. 
		ConfigItem* ptrClean = GPtrConfigItems; 
		GPtrConfigItems = (ConfigItem*)realloc(GPtrConfigItems, sizeof(ConfigItem) * ((++lineCount) + 1)); 
		
		if(GPtrConfigItems == NULL)
		{
			assert(ptrClean != NULL);
			free(ptrClean); 
			reportWin32Error(L"Unable to reallocate memory"); 
			CleanupConfigReader();
			return ERROR_INVALID_BLOCK; //TODO This might not be the best error code to return here 
		}
	}
	
	GPtrConfigItemsCount = lineCount; 
	
	for(size_t i = 0; i < GPtrConfigItemsCount; i++) { 
		printf("Config Item: %zi - %s: %s\n", i, GPtrConfigItems[i].name, GPtrConfigItems[i].value); 
	}
	
	fclose(configFileHandle); 
	
	return ERROR_SUCCESS;
}

void GetConfigFilePath() 
{ 
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



uint8_t LoadConfigFile() 
{ 
	
	GetConfigFilePath();

	if(!PathFileExistsW(*szPath))
	{
		SetLastError(ERROR_FILE_NOT_FOUND);
		reportWin32Error(L"Config file could not be found"); 
		return ERROR_FILE_NOT_FOUND;
	}
	
	ReadConfigFile();

	return ERROR_SUCCESS; 
}

void CleanupConfigReader() 
{ 
	CoTaskMemFree(szPath); 
	
	if(GPtrConfigItems) //TODO Cleanup the name and value pointers for the strings
	{
		for(size_t i = 0; i < GPtrConfigItemsCount; i++) 
		{
			if(&GPtrConfigItems[i] != NULL) 
			{ 
				if(GPtrConfigItems[i].name) 
				{ 
					free(GPtrConfigItems[i].name); 
				}
				
				if(GPtrConfigItems[i].value)
				{
					free(GPtrConfigItems[i].value); 
				}
			}
		}
		free(GPtrConfigItems); 
	}
}