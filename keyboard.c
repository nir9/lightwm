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
	
	hotKeyWindow = hwnd; 
	
	for(size_t i = 0; i < configItems->configItemsCount; i++) { 
		//Unrolled secondary for loop with if statements is more efficient but more annoying
		if(strcmp(configItems->configItem[i].name, ACTION_STRINGS[WORKSPACE_1]) == 0) 
		{ 
			AddKeyboardKeybind(
				WORKSPACE_1, 
				GetModifier(configItems->configItem[i].value), 
				GetKeyCode(configItems->configItem[i].value)
			);
			
			puts("Add Workspace 1 keybind"); 
		}
		if(strcmp(configItems->configItem[i].name, ACTION_STRINGS[WORKSPACE_2]) == 0) 
		{ 
			AddKeyboardKeybind(
				WORKSPACE_2, 
				GetModifier(configItems->configItem[i].value), 
				GetKeyCode(configItems->configItem[i].value)
			);
			
			puts("Add workspace 2 keybind"); 
		}
		if(strcmp(configItems->configItem[i].name, ACTION_STRINGS[WORKSPACE_3]) == 0) 
		{
			AddKeyboardKeybind(
				WORKSPACE_3, 
				GetModifier(configItems->configItem[i].value), 
				GetKeyCode(configItems->configItem[i].value)
			);
			
			puts("Add workspace 3 keybind"); 
		}
		if(strcmp(configItems->configItem[i].name, ACTION_STRINGS[WORKSPACE_4]) == 0) 
		{
			AddKeyboardKeybind(
				WORKSPACE_4, 
				GetModifier(configItems->configItem[i].value), 
				GetKeyCode(configItems->configItem[i].value)
			);
			
			puts("Add workspace 4 keybind"); 
		}
		if(strcmp(configItems->configItem[i].name, ACTION_STRINGS[WINDOW_UP]) == 0) 
		{
			AddKeyboardKeybind(
				WINDOW_UP, 
				GetModifier(configItems->configItem[i].value), 
				GetKeyCode(configItems->configItem[i].value)
			);
			
			puts("Add window up keybind"); 
		}
		if(strcmp(configItems->configItem[i].name, ACTION_STRINGS[WINDOW_DOWN]) == 0) 
		{
			AddKeyboardKeybind(
				WINDOW_DOWN, 
				GetModifier(configItems->configItem[i].value), 
				GetKeyCode(configItems->configItem[i].value)
			);
			
			puts("Add window down keybind"); 
		}
		if(strcmp(configItems->configItem[i].name, ACTION_STRINGS[WINDOW_LEFT]) == 0) 
		{
			AddKeyboardKeybind(
				WINDOW_LEFT, 
				GetModifier(configItems->configItem[i].value), 
				GetKeyCode(configItems->configItem[i].value)
			);
			
			puts("Add window left keybind"); 
		}
		if(strcmp(configItems->configItem[i].name, ACTION_STRINGS[WINDOW_RIGHT]) == 0) 
		{
			AddKeyboardKeybind(
				WINDOW_RIGHT, 
				GetModifier(configItems->configItem[i].value), 
				GetKeyCode(configItems->configItem[i].value)
			);
			
			puts("Add window right keybind"); 
		}
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
	strip(value); 
	removeControlChars(value); 
	
	printf("Len: %lu %s\n", strlen(value), value); 
	
	size_t valueLength = strlen(value) - 1; 
	return value[valueLength];
}

void AddKeyboardKeybind(enum Action action, uint8_t modifier, char keyCode)
{
	if(!RegisterHotKey(hotKeyWindow, action, modifier | MOD_NOREPEAT, keyCode)) 
	{ 
		if(GetLastError() == ERROR_HOTKEY_ALREADY_REGISTERED) 
		{
			puts("Warn: Hotkey already registerd");
			return; 
		}
		
		MessageBox(NULL, "Failed to register hotkey.", "Error", MB_OK | MB_ICONERROR);
	}
}