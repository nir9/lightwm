#include <Windows.h> 
#include <stdint.h> 
#include <assert.h>
#include <stdio.h>
 
#include "keyboard.h"

#include "debug.h" 

UINT getModifier(char* value); 
UINT getKeyCode(char* value); 
void addKeyboardKeybind(enum Action action, UINT modifier, UINT keyCode); 

BOOL initializeKeyboardConfig(ConfigItems* configItems) 
{ 
	assert(configItems != NULL); 
	assert(configItems->configItem != NULL); 
	assert(configItems->configItemsCount != 0);
	
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

void cleanupKeyboard() 
{
	UnregisterHotKey(NULL, WORKSPACE_1); 
	UnregisterHotKey(NULL, WORKSPACE_2); 
	UnregisterHotKey(NULL, WORKSPACE_3); 
	UnregisterHotKey(NULL, WORKSPACE_4); 
	UnregisterHotKey(NULL, WINDOW_UP); 
	UnregisterHotKey(NULL, WINDOW_DOWN); 
	UnregisterHotKey(NULL, WINDOW_LEFT); 
	UnregisterHotKey(NULL, WINDOW_RIGHT); 
}

UINT getModifier(char* value) 
{ 	
	// TODO handle the different modifier values
	// MOD_ALT
	// MOD_CONTROL
	// MOD_SHIFT
	// MOD_WIN
	return MOD_ALT; 
}

UINT getKeyCode(char* value) 
{ 
	DEBUG_PRINT("GetKeyCode char value '%c'", value[strlen(value) - 1]);
	return VkKeyScanEx(value[strlen(value) - 1], GetKeyboardLayout(0));
}

void addKeyboardKeybind(enum Action action, UINT modifier, UINT keyCode)
{
	if(!RegisterHotKey(NULL, action, modifier | MOD_NOREPEAT, keyCode)) 
	{ 
		if(GetLastError() == ERROR_HOTKEY_ALREADY_REGISTERED) 
		{
			puts("Warn: Hotkey already registerd\n");
			return; 
		}
		
		MessageBox(NULL, "Failed to register hotkey.", "Error", MB_OK | MB_ICONERROR);
	}
	
	DEBUG_PRINT("Registered %s hotkey", ACTION_STRINGS[action]);  
}