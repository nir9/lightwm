#pragma once 

#include <Windows.h> 
#include <stdint.h> 

#include "config.h" 
#include "actions.h" 

BOOL initializeKeyboardConfig(ConfigItems* configItems); 
void cleanupKeyboard(); 
LRESULT handleHotkey(WPARAM lparam, LPARAM wparam); 
