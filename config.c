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
#include "string_helpers.h"
#include "debug.h"

#define BUFF_SIZE 65536

PWSTR szConfigFilePath;

char *defaultConfigData = NULL;

ConfigItems *configItems;

BOOL createDefaultConfigFile(HINSTANCE);

BOOL loadDefaultConfigResourceData(HINSTANCE);

BOOL writeDefaultConfigDataToFile();

size_t getLineCount(FILE *file);

void freeConfigItems(ConfigItems *items);

DWORD readConfigFile() {
    assert(szConfigFilePath != NULL);

    //Try to open the config file based on the path
    FILE *configFileHandle = _wfopen(szConfigFilePath, L"r");

    if (configFileHandle == NULL) {
        reportWin32Error(L"Config file could not be opened");
        cleanupConfigReader();
        return ERROR_INVALID_HANDLE;
    }

    char line[BUFF_SIZE];

    configItems = (ConfigItems *) malloc(sizeof(ConfigItems));

    configItems->configItem = (ConfigItem *) malloc(sizeof(ConfigItem) * getLineCount(configFileHandle) + 1);
    configItems->configItemsCount = getLineCount(configFileHandle);

    if (configItems->configItem == NULL) {
        reportWin32Error(L"Allocation ConfigItem memory");
        cleanupConfigReader();
        return ERROR_NOT_ENOUGH_MEMORY;
    }

    for (size_t lineCount = 0; fgets(line, sizeof(line), configFileHandle); lineCount++) {
        if (strlen(line) == 0) {
            continue;
        }

        //Get the first half of the line
        char *token = strtok(line, " ");
        configItems->configItem[lineCount].name = (char *) malloc(strlen(token) + 1);
        strncpy(configItems->configItem[lineCount].name, token, strlen(token) + 1);

        //Get the second half of the line
        token = strtok(NULL, " ");
        removeControlChars(token);
        configItems->configItem[lineCount].value = (char *) malloc(strlen(token) + 1);
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

void getConfigFilePath() {
    //TODO: We don't check other results possible results, i.e E_FAIL
    const HRESULT getAppDataPathResult = SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, NULL, &szConfigFilePath);

    if (!SUCCEEDED(getAppDataPathResult)) {
        switch (getAppDataPathResult) {
            case E_FAIL:
                SetLastError(ERROR_DIRECTORY_NOT_SUPPORTED);
                break;
            case E_INVALIDARG:
                SetLastError(ERROR_TIERING_INVALID_FILE_ID);
                break;
            default:
                SetLastError(ERROR_GEN_FAILURE);
        }
        reportWin32Error(L"Could not get the users appdata directory");
        cleanupConfigReader();
        exit(ERROR_PATH_NOT_FOUND);
    }

    assert(szConfigFilePath != NULL);

    //TODO just found a bug that causes the path to become corrupt
    const HRESULT concatStringResult = StringCchCatW(szConfigFilePath, MAX_PATH, L"\\lightwm.config\0");

    if (!SUCCEEDED(concatStringResult)) {
        switch (concatStringResult) {
            case STRSAFE_E_INVALID_PARAMETER:
                SetLastError(ERROR_INVALID_PARAMETER);
                break;
            case STRSAFE_E_INSUFFICIENT_BUFFER:
                SetLastError(ERROR_INSUFFICIENT_BUFFER);
                break;
            default:
                SetLastError(ERROR_GEN_FAILURE);
        }
        reportWin32Error(L"Could not append file name to appdata path");
        cleanupConfigReader();
        exit(GetLastError());
    }
}

BOOL loadConfigFile(HINSTANCE resourceModuleHandle) {
    szConfigFilePath = (PWSTR) malloc(sizeof(WCHAR) * MAX_PATH);
    getConfigFilePath();
    assert(szConfigFilePath != NULL);

    if (!PathFileExistsW(szConfigFilePath)) {
        if (!createDefaultConfigFile(resourceModuleHandle)) {
            reportWin32Error(L"Create a default config file");
            return FALSE;
        }
    }

    readConfigFile();

    return TRUE;
}

void freeConfigItems(ConfigItems *items) {
    if (items != NULL) {
        for(size_t i = 0; i < items->configItemsCount; i++) {
            free(items->configItem[i].name);
            free(items->configItem[i].value);
        }
        free(items);
    } else {
        DEBUG_PRINT("items ptr was freed earlier!");
    }
}

void cleanupConfigReader() {
    freeConfigItems(configItems);
    DEBUG_PRINT("Cleaned up config items");

    CoTaskMemFree(szConfigFilePath);
    DEBUG_PRINT("Cleaned up filePath ptr");
}

ConfigItems *getConfigItems() {
    return configItems;
}

/**
 * Private definitions here
 **/
BOOL createDefaultConfigFile(HINSTANCE resourceModuleHandle) {
    if (!loadDefaultConfigResourceData(resourceModuleHandle)) {
        return FALSE;
    }

    if (!writeDefaultConfigDataToFile()) {
        return FALSE;
    }

    return TRUE;
}

BOOL loadDefaultConfigResourceData(HINSTANCE resourceModuleHandle) {
    const HRSRC hRes = FindResource(resourceModuleHandle, MAKEINTRESOURCE(IDR_DEFAULT_CONFIG), RT_RCDATA);

    if (hRes == NULL) {
        puts("Could not get HRSRC Handle");
        DEBUG_PRINT("FindResource Error: %lu", GetLastError());
        return FALSE;
    }

    const HGLOBAL hData = LoadResource(resourceModuleHandle, hRes);

    if (hData == NULL) {
        puts("Could not load resource");
        DEBUG_PRINT("LoadResource Error: %lu", GetLastError());
        return FALSE;
    }

    const LPVOID defaultConfigResourceData = LockResource(hData);

    if (defaultConfigResourceData == NULL) {
        puts("Could not read resource");
        DEBUG_PRINT("LockResource Error: %lu", GetLastError());
        return FALSE;
    }

    size_t defaultConfigResourceDataLen = strlen(defaultConfigResourceData) + 1; //+1 for the null char
    defaultConfigData = (char *) malloc(sizeof(char) * defaultConfigResourceDataLen); //TODO Error checking
    strcpy(defaultConfigData, defaultConfigResourceData);

    return TRUE;
}

BOOL writeDefaultConfigDataToFile() {
    FILE *configFileHandle = _wfopen(szConfigFilePath, L"w");

    if (configFileHandle == NULL) {
        return FALSE;
    }

    fprintf(configFileHandle, defaultConfigData);

    puts("Created default config file");

    fclose(configFileHandle);

    return TRUE;
}

size_t getLineCount(FILE *file) {
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
