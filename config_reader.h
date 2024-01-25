/**
 * Config reader
 * This program will read the config for lightwm in the appdata directory
 * 
 * Demetry Romanowski
 * demetryromanowski@gmail.com
 **/ 
 
 #pragma once
 
 #include <stdint.h>
 
typedef struct KeyboardConfigItem { 
	char* name;
	uint16_t keycode;
} _KeyboardConfigItem; 

KeyboardConfigItem[256] KeyboardConfigArr; //TODO Maybe move this to heap memory
  
uint8_t LoadConfigFile(); 

uint8_t CreateDefaultConfigFile(); 

void CleanupConfigReader(); 