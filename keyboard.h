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

BOOL InitializeKeyboardConfig(HWND hwnd, ConfigItems* configItems); 
void CleanupKeyboard(); 
