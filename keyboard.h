#pragma once 

#include <Windows.h> 
#include <stdint.h> 

#include "config.h" 
#include "actions.h" 

BOOL InitializeKeyboardConfig(ConfigItems* configItems); 
void CleanupKeyboard(); 
