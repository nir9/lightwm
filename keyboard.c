#include <windows.h>
#include "keyboard.h"
#include "tiling.h"
#include "error.h"
#include "config.h"

UINT getKeyCode(char value)
{
    return VkKeyScanEx(value, GetKeyboardLayout(0));
}

void addKeyboardKeybind(int id, UINT keyCode)
{
    if (!RegisterHotKey(NULL, id, MOD_ALT | MOD_NOREPEAT, keyCode)) {
        reportWin32Error(L"Failed to register hotkey");
		return;
    }
}

bool initializeKeyboardConfig()
{
	addKeyboardKeybind(TOGGLE_FULLSCREEN_MODE_HOYKEY_ID, getKeyCode(FULLSCREEN_MODE_HOTKEY));
	addKeyboardKeybind(NEXT_WINDOW_HOTKEY_ID, getKeyCode(NEXT_WINDOW_HOTKEY));
	addKeyboardKeybind(PREV_WINDOW_HOTKEY_ID, getKeyCode(PREV_WINDOW_HOTKEY));
	addKeyboardKeybind(QUIT_LIGHTWM_HOTKEY_ID, getKeyCode(QUIT_LIGHTWM_HOTKEY));
	addKeyboardKeybind(FORCE_TILE_LIGHTWM_HOTKEY_ID, getKeyCode(FORCE_TILE_LIGHTWM_HOTKEY));
	for (int i = 0; i < 9; i++) {
		addKeyboardKeybind(WORKSPACE_LIGHTWM_HOTKEY_ID_BASE + i, getKeyCode(WORKSPACE_LIGHTWM_HOTKEY_BASE + i));
	}

    return true;
}

void cleanupKeyboard()
{
    UnregisterHotKey(NULL, TOGGLE_FULLSCREEN_MODE_HOYKEY_ID);
    UnregisterHotKey(NULL, NEXT_WINDOW_HOTKEY_ID);
    UnregisterHotKey(NULL, PREV_WINDOW_HOTKEY_ID);
    UnregisterHotKey(NULL, QUIT_LIGHTWM_HOTKEY_ID);
	UnregisterHotKey(NULL, FORCE_TILE_LIGHTWM_HOTKEY_ID);
	for (int i = 0; i < 9; i++) {
		UnregisterHotKey(NULL, WORKSPACE_LIGHTWM_HOTKEY_ID_BASE + i);
	}
}

void handleHotkey(WPARAM wparam, LPARAM lparam)
{
    switch (wparam) {
		case TOGGLE_FULLSCREEN_MODE_HOYKEY_ID:
			toggleFullscreenMode();
			break;
		case PREV_WINDOW_HOTKEY_ID:
			focusNextWindow(true);
			break;
		case NEXT_WINDOW_HOTKEY_ID:
			focusNextWindow(false);
			break;
		case FORCE_TILE_LIGHTWM_HOTKEY_ID:
			tileWindows();
			break;
    }

	if (wparam >= WORKSPACE_LIGHTWM_HOTKEY_ID_BASE && wparam <= (WORKSPACE_LIGHTWM_HOTKEY_ID_BASE + 8)) {
		gotoWorkspace((int)wparam - WORKSPACE_LIGHTWM_HOTKEY_ID_BASE + 1);
	}
}
