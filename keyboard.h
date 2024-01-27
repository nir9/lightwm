/**
 * keyboard.c Handles the keyboard controls
 * 
 * Demetry Romanowski
 * demetryromanowski@gmail.com
 **/ 
 
#pragma once 

#include <Windows.h> 
#include <stdint.h> 

#include "config.h" 
#include "actions.h" 

typedef struct _KeyboardConfigItem KeyboardConfigItem; 
typedef struct _KeyboardConfigItems KeyboardConfigItems; 

typedef struct _KeyboardConfigItem {
	Actions action;
	uint8_t modifier; 
	uint8_t keyCode;
} KeyboardConfigItem; 

typedef struct _KeyboardConfigItems {
	KeyboardConfigItem* keyboardConfigItem; 
	size_t keyboardConfigItemsCount; 
} KeyboardConfigItems; 

BOOL InitializeKeyboardConfig(ConfigItems* configItems); 
BOOL IsKeyboardAction(uint8_t keyCode);
void CleanupKeyboard(); 
