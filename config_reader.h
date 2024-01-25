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
  
uint8_t LoadConfigFile(); 

uint8_t CreateDefaultConfigFile(); 