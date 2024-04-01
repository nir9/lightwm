#pragma once 

#include <Windows.h> 
#include <stdint.h> 
#include <stdbool.h>

bool initializeKeyboardConfig(); 
void cleanupKeyboard(); 
void handleHotkey(WPARAM lparam, LPARAM wparam); 
