#include "config.h"

#include <Shlobj.h>
#include <wchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <shlwapi.h>
#include <assert.h>

#include "error.h"
#include "resource.h"
#include "string_helpers.h"
#include "debug.h"

#define BUFF_SIZE 65536

PWSTR configPath;

char* defaultConfigData = NULL;

ConfigItems* configItems;

bool createDefaultConfigFile(HINSTANCE);

bool loadDefaultConfigResourceData(HINSTANCE);

bool writeDefaultConfigDataToFile();

size_t getLineCount(FILE* file);

void freeConfigItems(ConfigItems* items);

bool readConfigFile()
{
    FILE* configFileHandle = _wfopen(configPath, L"r");

    if (configFileHandle == NULL) {
        reportGeneralError(L"Config file could not be opened");
		goto failure;
    }

    char line[BUFF_SIZE];

    configItems = (ConfigItems*)malloc(sizeof(ConfigItems));

	const numOfLinesInConfig = getLineCount(configFileHandle);

    configItems->configItem = (ConfigItem*)malloc(sizeof(ConfigItem) * numOfLinesInConfig + 1);
    configItems->configItemsCount = numOfLinesInConfig;

    if (configItems->configItem == NULL) {
        reportGeneralError(L"Allocation ConfigItem memory");
		goto failure;
    }

    for (size_t lineCount = 0; fgets(line, sizeof(line), configFileHandle); lineCount++) {
        if (strlen(line) == 0) {
            continue;
        }

        char* token = strtok(line, " ");
        configItems->configItem[lineCount].name = (char*) malloc(strlen(token) + 1);
        strncpy(configItems->configItem[lineCount].name, token, strlen(token) + 1);

        token = strtok(NULL, " ");
        removeControlChars(token);
        configItems->configItem[lineCount].value = (char*) malloc(strlen(token) + 1);
        strncpy(configItems->configItem[lineCount].value, token, strlen(token) + 1);
    }

    fclose(configFileHandle);

	return true;

failure:
	cleanupConfigReader();
	return false;
}

bool initConfigFilePath()
{
    const HRESULT getAppDataPathResult = SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, NULL, &configPath);

    if (getAppDataPathResult != S_OK) {
        reportGeneralError(L"Could not get the users app data directory");
		return false;
    }

    // TODO: Check if there may be a bug that causes the path to become corrupt
    const HRESULT concatStringResult = StringCchCatW(configPath, MAX_PATH, L"\\lightwm.config\0");

    if (FAILED(concatStringResult)) {
        reportGeneralError(L"Could not append file name to app data path");
		return false;
    }

	return true;
}

bool loadConfigFile(HINSTANCE resourceModuleHandle) {
    if (!initConfigFilePath()) {
		return false;
	}

    if (!PathFileExistsW(configPath)) {
        if (!createDefaultConfigFile(resourceModuleHandle)) {
            reportWin32Error(L"Create a default config file");
            return false;
        }
    }

    return readConfigFile();
}

void freeConfigItems(ConfigItems *items) {
    if (items != NULL) {
        for(size_t i = 0; i < items->configItemsCount; i++) {
            free(items->configItem[i].name);
            free(items->configItem[i].value);
        }

        free(items);
    }
}

void cleanupConfigReader() {
    freeConfigItems(configItems);
    CoTaskMemFree(configPath);
}

ConfigItems* getConfigItems() {
    return configItems;
}

bool createDefaultConfigFile(HINSTANCE resourceModuleHandle) {
    if (!loadDefaultConfigResourceData(resourceModuleHandle)) {
        return false;
    }

    if (!writeDefaultConfigDataToFile()) {
        return false;
    }

    return true;
}

bool loadDefaultConfigResourceData(HINSTANCE resourceModuleHandle) {
    const HRSRC hRes = FindResource(resourceModuleHandle, MAKEINTRESOURCE(IDR_DEFAULT_CONFIG), RT_RCDATA);

    if (hRes == NULL) {
        reportWin32Error(L"FindResource Error");
        return false;
    }

    const HGLOBAL hData = LoadResource(resourceModuleHandle, hRes);

    if (hData == NULL) {
        reportWin32Error(L"LoadResource Error");
        return false;
    }

    const LPVOID defaultConfigResourceData = LockResource(hData);

    if (defaultConfigResourceData == NULL) {
        reportWin32Error(L"LockResource Error");
        return false;
    }

	int nullTerm = 1;
    size_t defaultConfigResourceDataLen = strlen(defaultConfigResourceData) + nullTerm;
    defaultConfigData = (char*)malloc(sizeof(char) * defaultConfigResourceDataLen); // TODO Error checking
    strcpy(defaultConfigData, defaultConfigResourceData);

    return true;
}

bool writeDefaultConfigDataToFile() {
    FILE* configFileHandle = _wfopen(configPath, L"w");

    if (configFileHandle == NULL) {
        return false;
    }

    fprintf(configFileHandle, defaultConfigData);

    fclose(configFileHandle);

    return true;
}

size_t getLineCount(FILE* file) {
    int counter = 0;
    for (;;) {
        char buf[BUFF_SIZE];
        const size_t res = fread(buf, 1, BUFF_SIZE, file);
        if (ferror(file)) {
            return -1;
        }

        for (int i = 0; i < res; i++) {
            if (buf[i] == '\n') {
                counter++;
            }
        }

        if (feof(file)) {
            break;
        }
    }

    fseek(file, 0, SEEK_SET);

    return counter;
}
