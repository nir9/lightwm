/**
 * Config Reader and Parser
 * This program will read the config for lightwm in the appdata directory
 * 
 * Demetry Romanowski
 * demetryromanowski@gmail.com
 **/ 
 
#pragma once
 
#include <stdint.h>

typedef struct _ConfigItem ConfigItem;
typedef struct _ConfigItems ConfigItems; 

typedef struct _ConfigItem { 
	char* name;
	char* value;
} ConfigItem; 

typedef struct _ConfigItems { 
	ConfigItem* configItem; 
	size_t configItemsCount;
} ConfigItems; 

uint8_t LoadConfigFile(HINSTANCE); 
void CleanupConfigReader(); 
ConfigItems* GetConfigItems(); 
