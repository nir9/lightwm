#pragma once 

#include <Windows.h> 
#include <stdint.h> 

#include "config.h" 
#include "actions.h" 

#define ADD_KEYBOARD_KEYBIND(i, action) \
if(strcmp(configItems->configItem[i].name, ACTION_STRINGS[action]) == 0) \
{ \
    addKeyboardKeybind( \
        action, \
        getModifier(configItems->configItem[i].value), \
        getKeyCode(configItems->configItem[i].value) \
    ); \
}

BOOL initializeKeyboardConfig(ConfigItems* configItems); 
void cleanupKeyboard(); 
LRESULT handleHotkey(WPARAM lparam, LPARAM wparam); 