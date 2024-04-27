#include <windows.h>
#include "keyboard.h"
#include "tiling.h"
#include "error.h"
#include "config.h"

UINT getModifier(const char* value)
{
    if (lstrcmpA(value, "alt") == 0) {
        return MOD_ALT;
    }

    if (lstrcmpA(value, "win") == 0) {
        return MOD_WIN;
    }

    if (lstrcmpA(value, "ctrl") == 0) {
        return MOD_CONTROL;
    }

    if (lstrcmpA(value, "shift") == 0) {
        return MOD_SHIFT;
    }

    return MOD_ALT;
}

UINT getKeyCode(const char* value)
{
    return VkKeyScanEx(value[lstrlenA(value) - 1], GetKeyboardLayout(0));
}

void addKeyboardKeybind(int id, UINT modifier, UINT keyCode)
{
    if (!RegisterHotKey(NULL, id, modifier | MOD_NOREPEAT, keyCode)) {
        reportWin32Error(L"Failed to register hotkey");
		return;
    }
}

bool initializeKeyboardConfig()
{
	addKeyboardKeybind(TOGGLE_FOCUS_MODE_HOYKEY_ID, getModifier(FOCUS_MODE_HOTKEY), getKeyCode(FOCUS_MODE_HOTKEY));
	addKeyboardKeybind(NEXT_WINDOW_HOTKEY_ID, getModifier(NEXT_WINDOW_HOTKEY), getKeyCode(NEXT_WINDOW_HOTKEY));
	addKeyboardKeybind(PREV_WINDOW_HOTKEY_ID, getModifier(PREV_WINDOW_HOTKEY), getKeyCode(PREV_WINDOW_HOTKEY));
	addKeyboardKeybind(QUIT_LIGHTWM_HOTKEY_ID, getModifier(QUIT_LIGHTWM_HOTKEY), getKeyCode(QUIT_LIGHTWM_HOTKEY));

    return true;
}

void cleanupKeyboard()
{
    UnregisterHotKey(NULL, TOGGLE_FOCUS_MODE_HOYKEY_ID);
    UnregisterHotKey(NULL, NEXT_WINDOW_HOTKEY_ID);
    UnregisterHotKey(NULL, PREV_WINDOW_HOTKEY_ID);
    UnregisterHotKey(NULL, QUIT_LIGHTWM_HOTKEY_ID);
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
        default:
            break;
    }
}
