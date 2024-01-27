/**
 * keyboard.c Handles the keyboard controls
 * 
 * Demetry Romanowski
 * demetryromanowski@gmail.com
 **/ 
 
 #pragma once 

#include <Windows.h> 

#include <stdint.h> 

typedef struct _KeyboardConfigItem KeyboardConfigItem; 
typedef struct _KeyboardConfigItems KeyboardConfigItems; 
 
BOOL IsKeyboardAction(uint8_t keyCode);
