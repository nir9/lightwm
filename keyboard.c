/**
 * keyboard.c Handles the keyboard controls
 * 
 * Demetry Romanowski
 * demetryromanowski@gmail.com
 **/ 
#include <Windows.h> 
#include <stdint.h> 
#include <assert.h>
#include <stdio.h>
 
#include "keyboard.h"

#include "debug.h" 

//Keyboard vars

//Private definitions
uint8_t GetModifier(char* value); 
char GetKeyCode(char* value); 
void AddKeyboardKeybind(enum Action action, uint8_t modifier, uint8_t keyCode); 

HWND hotKeyWindow;

//Public implementations
BOOL InitializeKeyboardConfig(HWND hwnd, ConfigItems* configItems) 
{ 
	assert(configItems != NULL); 
	assert(configItems->configItem != NULL); 
	assert(configItems->configItemsCount != 0);
	assert(hwnd != NULL);
	
	hotKeyWindow = hwnd; 
	
	for(size_t i = 0; i < configItems->configItemsCount; i++) { 
		//TODO Modify the macro to use generate all these automatically.
		ADD_KEYBOARD_KEYBIND(i, WORKSPACE_1);
		ADD_KEYBOARD_KEYBIND(i, WORKSPACE_2);
		ADD_KEYBOARD_KEYBIND(i, WORKSPACE_3);
		ADD_KEYBOARD_KEYBIND(i, WORKSPACE_4);
		ADD_KEYBOARD_KEYBIND(i, WINDOW_UP);
		ADD_KEYBOARD_KEYBIND(i, WINDOW_DOWN);
		ADD_KEYBOARD_KEYBIND(i, WINDOW_LEFT);
		ADD_KEYBOARD_KEYBIND(i, WINDOW_RIGHT);
	}
	
	return TRUE; 
}

void CleanupKeyboard() 
{
	UnregisterHotKey(hotKeyWindow, WORKSPACE_1); 
	UnregisterHotKey(hotKeyWindow, WORKSPACE_2); 
	UnregisterHotKey(hotKeyWindow, WORKSPACE_3); 
	UnregisterHotKey(hotKeyWindow, WORKSPACE_4); 
	UnregisterHotKey(hotKeyWindow, WINDOW_UP); 
	UnregisterHotKey(hotKeyWindow, WINDOW_DOWN); 
	UnregisterHotKey(hotKeyWindow, WINDOW_LEFT); 
	UnregisterHotKey(hotKeyWindow, WINDOW_RIGHT); 
}

//Private implementations
uint8_t GetModifier(char* value) 
{ 	
	// TODO 
	// MOD_ALT
	// MOD_CONTROL
	// MOD_NOREPEAT
	// MOD_SHIFT
	// MOD_WIN
	return MOD_ALT; 
}

char GetKeyCode(char* value) 
{ 
	// printf("DEBUG keyboard.c L146: %c\n", value[strlen(value) - 1]);
	return value[strlen(value) - 1];
}

void AddKeyboardKeybind(enum Action action, uint8_t modifier, char keyCode)
{
	if(!RegisterHotKey(hotKeyWindow, action, modifier | MOD_NOREPEAT, keyCode)) 
	{ 
		if(GetLastError() == ERROR_HOTKEY_ALREADY_REGISTERED) 
		{
			puts("Warn: Hotkey already registerd\n");
			return; 
		}
		
		MessageBox(NULL, "Failed to register hotkey.", "Error", MB_OK | MB_ICONERROR);
	}
	
	DEBUG_PRINT("DEBUG keyboard.c: Registered %s hotkey\n", ACTION_STRINGS[action]);  
}