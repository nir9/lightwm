#pragma once 

#include <Windows.h> 
#include <stdint.h> 
#include <stdbool.h>
#include "config.h" 
#include "actions.h" 

bool initializeKeyboardConfig(ConfigItems* configItems); 
void cleanupKeyboard(); 
void handleHotkey(WPARAM lparam, LPARAM wparam); 
