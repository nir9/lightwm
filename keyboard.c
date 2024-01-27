/**
 * keyboard.c Handles the keyboard controls
 * 
 * Demetry Romanowski
 * demetryromanowski@gmail.com
 **/ 
#include <Windows.h> 
#include <stdint.h> 
#include <assert.h> 
 
#include "keyboard.h"
#include "actions.h"
#include "config.h" 

//Keyboard global vars
KeyboardConfigItems keyboardConfigItems; 

//Private definitions
BOOL CheckModifiers(char* value); 

//Public implementations
BOOL InitializeKeyboardConfig(ConfigItems* configItems) 
{ 
	assert(configItems != NULL); 
	assert(configItems->configItem != NULL); 
	assert(configItems->configItemsCount != 0); 
	
	for(size_t i = 0; i < configItems->configItemsCount; i++) { 
		if(strcmp(configItems->configItem[i].name, ACTION_STRINGS[WORKSPACE_1]) == 0) 
		{ 
			puts("Add Workspace 1 keybind"); 
		}
		if(strcmp(configItems->configItem[i].name, ACTION_STRINGS[WORKSPACE_2]) == 0) 
		{ 
			puts("Add workspace 2 keybind"); 
		}
		if(strcmp(configItems->configItem[i].name, ACTION_STRINGS[WORKSPACE_3]) == 0) 
		{ 
			puts("Add workspace 3 keybind"); 
		}
		if(strcmp(configItems->configItem[i].name, ACTION_STRINGS[WORKSPACE_4]) == 0) 
		{ 
			puts("Add workspace 4 keybind"); 
		}
		if(strcmp(configItems->configItem[i].name, ACTION_STRINGS[WINDOW_UP]) == 0) 
		{ 
			puts("Add window up keybind"); 
		}
		if(strcmp(configItems->configItem[i].name, ACTION_STRINGS[WINDOW_DOWN]) == 0) 
		{ 
			puts("Add window down keybind"); 
		}
		if(strcmp(configItems->configItem[i].name, ACTION_STRINGS[WINDOW_LEFT]) == 0) 
		{ 
			puts("Add window left keybind"); 
		}
		if(strcmp(configItems->configItem[i].name, ACTION_STRINGS[WINDOW_RIGHT]) == 0) 
		{ 
			puts("Add window right keybind"); 
		}
	}
	
	return TRUE; 
}

BOOL IsKeyboardAction(uint8_t keyCode)
{
	return TRUE;
}

BOOL CheckModifiers(char* value) 
{ 
	return strchr(value, '+') != NULL; 
}

void CleanupKeyboard() 
{
	if(keyboardConfigItems.keyboardConfigItem != NULL) 
	{
		free(keyboardConfigItems.keyboardConfigItem);
	}
}

//Private implementations