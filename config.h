#pragma once

#include <Windows.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct _ConfigItem {
    char* name;
    char* value;
} ConfigItem;

typedef struct _ConfigItems {
    ConfigItem* configItem;
    size_t configItemsCount;
} ConfigItems;

bool loadConfigFile(HINSTANCE);

void cleanupConfigReader();

ConfigItems* getConfigItems();
