/**
 * Config reader
 * This program will read the config for lightwm in the appdata directory
 * 
 * Demetry Romanowski
 * demetryromanowski@gmail.com
 **/ 
 
#pragma once
 
#include <stdint.h>

typedef struct _ConfigItem ConfigItem;
typedef struct _ConfigItems ConfigItems; 

uint8_t LoadConfigFile(); 

uint8_t CreateDefaultConfigFile(); 

void CleanupConfigReader(); 