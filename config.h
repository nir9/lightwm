#pragma once
#include <Windows.h>
#include <stdint.h>

typedef struct _ConfigItem ConfigItem;
typedef struct _ConfigItems ConfigItems;

typedef struct _ConfigItem {
    char *name;
    char *value;
} ConfigItem;

typedef struct _ConfigItems {
    ConfigItem *configItem;
    size_t configItemsCount;
} ConfigItems;

BOOL loadConfigFile(HINSTANCE);

void cleanupConfigReader();

ConfigItems *getConfigItems();
