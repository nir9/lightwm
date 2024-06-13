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
	addKeyboardKeybind(TOGGLE_FOCUS_MODE_HOYKEY_ID, getKeyCode(FOCUS_MODE_HOTKEY));
	addKeyboardKeybind(NEXT_WINDOW_HOTKEY_ID, getKeyCode(NEXT_WINDOW_HOTKEY));
	addKeyboardKeybind(PREV_WINDOW_HOTKEY_ID, getKeyCode(PREV_WINDOW_HOTKEY));
	addKeyboardKeybind(QUIT_LIGHTWM_HOTKEY_ID, getKeyCode(QUIT_LIGHTWM_HOTKEY));
	addKeyboardKeybind(WORKSPACE_1_LIGHTWM_HOTKEY_ID, getKeyCode(WORKSPACE_1_LIGHTWM_HOTKEY));
	addKeyboardKeybind(WORKSPACE_2_LIGHTWM_HOTKEY_ID, getKeyCode(WORKSPACE_2_LIGHTWM_HOTKEY));
	addKeyboardKeybind(WORKSPACE_3_LIGHTWM_HOTKEY_ID, getKeyCode(WORKSPACE_3_LIGHTWM_HOTKEY));

    return true;
}

void cleanupKeyboard()
{
    UnregisterHotKey(NULL, TOGGLE_FOCUS_MODE_HOYKEY_ID);
    UnregisterHotKey(NULL, NEXT_WINDOW_HOTKEY_ID);
    UnregisterHotKey(NULL, PREV_WINDOW_HOTKEY_ID);
    UnregisterHotKey(NULL, QUIT_LIGHTWM_HOTKEY_ID);
    UnregisterHotKey(NULL, WORKSPACE_1_LIGHTWM_HOTKEY_ID);
    UnregisterHotKey(NULL, WORKSPACE_2_LIGHTWM_HOTKEY_ID);
    UnregisterHotKey(NULL, WORKSPACE_3_LIGHTWM_HOTKEY_ID);
}

void handleHotkey(WPARAM wparam, LPARAM lparam)
{
    switch (wparam) {
		case TOGGLE_FOCUS_MODE_HOYKEY_ID:
			toggleFocusedWindow(GetForegroundWindow());
			break;
		case PREV_WINDOW_HOTKEY_ID:
			focusNextWindow(true);
			break;
		case NEXT_WINDOW_HOTKEY_ID:
			focusNextWindow(false);
			break;
		case WORKSPACE_1_LIGHTWM_HOTKEY_ID:
			gotoWorkspace(1);
			break;
		case WORKSPACE_2_LIGHTWM_HOTKEY_ID:
			gotoWorkspace(2);
			break;
		case WORKSPACE_3_LIGHTWM_HOTKEY_ID:
			gotoWorkspace(3);
			break;
    }
}
