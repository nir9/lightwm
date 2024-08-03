#include <windows.h>
#include "keyboard.h"
#include "tiling.h"
#include "error.h"
#include "config.h"

UINT getKeyCode(char value)
{
	return VkKeyScanEx(value, GetKeyboardLayout(0));
}

void addKeyboardKeybind(int id, UINT keyCode, UINT modifiers)
{
	if (!RegisterHotKey(NULL, id, modifiers, keyCode)) {
		reportWin32Error(L"Failed to register hotkey");
	}
}


bool initializeKeyboardConfig()
{
	addKeyboardKeybind(TOGGLE_FULLSCREEN_MODE_HOTKEY_ID, getKeyCode(FULLSCREEN_MODE_HOTKEY), MOD_ALT | MOD_NOREPEAT);
	addKeyboardKeybind(NEXT_WINDOW_HOTKEY_ID, getKeyCode(NEXT_WINDOW_HOTKEY), MOD_ALT | MOD_NOREPEAT);
	addKeyboardKeybind(PREV_WINDOW_HOTKEY_ID, getKeyCode(PREV_WINDOW_HOTKEY), MOD_ALT | MOD_NOREPEAT);
	addKeyboardKeybind(QUIT_LIGHTWM_HOTKEY_ID, getKeyCode(QUIT_LIGHTWM_HOTKEY), MOD_ALT | MOD_NOREPEAT);
	addKeyboardKeybind(FORCE_TILE_LIGHTWM_HOTKEY_ID, getKeyCode(FORCE_TILE_LIGHTWM_HOTKEY), MOD_ALT | MOD_NOREPEAT);
	
	for (int i = 0; i < 9; i++) {
		addKeyboardKeybind(WORKSPACE_LIGHTWM_HOTKEY_ID_BASE + i, getKeyCode('1' + i), MOD_ALT | MOD_NOREPEAT);
	}
	
	for (int i = 0; i < 9; i++) {
		addKeyboardKeybind(SWITCH_WORKSPACE_HOTKEY_ID + i, getKeyCode('1' + i), MOD_ALT | MOD_SHIFT | MOD_NOREPEAT);
	}

	return true;
}

void cleanupKeyboard()
{
	UnregisterHotKey(NULL, TOGGLE_FULLSCREEN_MODE_HOTKEY_ID);
	UnregisterHotKey(NULL, NEXT_WINDOW_HOTKEY_ID);
	UnregisterHotKey(NULL, PREV_WINDOW_HOTKEY_ID);
	UnregisterHotKey(NULL, QUIT_LIGHTWM_HOTKEY_ID);
	UnregisterHotKey(NULL, FORCE_TILE_LIGHTWM_HOTKEY_ID);
	
	for (int i = 0; i < 9; i++) {
		UnregisterHotKey(NULL, WORKSPACE_LIGHTWM_HOTKEY_ID_BASE + i);
		UnregisterHotKey(NULL, SWITCH_WORKSPACE_HOTKEY_ID + i);
	}
}

void handleHotkey(WPARAM wparam, LPARAM lparam)
{
	switch (wparam) {
		case TOGGLE_FULLSCREEN_MODE_HOTKEY_ID:
			toggleFullscreenMode();
			break;
		case NEXT_WINDOW_HOTKEY_ID:
			focusNextWindow(false);
			break;
		case PREV_WINDOW_HOTKEY_ID:
			focusNextWindow(true);
			break;
		case FORCE_TILE_LIGHTWM_HOTKEY_ID:
			tileWindows();
			break;
	}

	if (wparam >= WORKSPACE_LIGHTWM_HOTKEY_ID_BASE && wparam < WORKSPACE_LIGHTWM_HOTKEY_ID_BASE + 9) {
		gotoWorkspace((int)wparam - WORKSPACE_LIGHTWM_HOTKEY_ID_BASE + 1);
	} else if (wparam >= SWITCH_WORKSPACE_HOTKEY_ID && wparam < SWITCH_WORKSPACE_HOTKEY_ID + 9) {
		moveWindowToWorkspace((int)wparam - SWITCH_WORKSPACE_HOTKEY_ID + 1);
	}
}
