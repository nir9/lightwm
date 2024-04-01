#include <Windows.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include "keyboard.h"
#include "tiling.h"
#include "debug.h"
#include "error.h"
#include "config.h"

#define TOGGLE_FOCUS_MODE_HOYKEY_ID 0
#define NEXT_WINDOW_HOTKEY_ID 1
#define PREV_WINDOW_HOTKEY_ID 2

UINT getModifier(const char* value)
{
    if (strncmp(value, "alt", 3) == 0) {
        return MOD_ALT;
    }

    if (strncmp(value, "win", 3) == 0) {
        return MOD_WIN;
    }

    if (strncmp(value, "ctrl", 4) == 0) {
        return MOD_CONTROL;
    }

    if (strncmp(value, "shift", 5) == 0) {
        return MOD_SHIFT;
    }
    return MOD_ALT; // TODO Throw an error here
}

UINT getKeyCode(const char *value)
{
    DEBUG_PRINT("GetKeyCode char value '%c'", value[strlen(value) - 1]);
    return VkKeyScanEx(value[strlen(value) - 1], GetKeyboardLayout(0));
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

    return true;
}

void cleanupKeyboard()
{
    UnregisterHotKey(NULL, TOGGLE_FOCUS_MODE_HOYKEY_ID);
    UnregisterHotKey(NULL, NEXT_WINDOW_HOTKEY_ID);
    UnregisterHotKey(NULL, PREV_WINDOW_HOTKEY_ID);
}

void handleHotkey(WPARAM wparam, LPARAM lparam) {
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
            DEBUG_PRINT("Unhandled hotkey message! Hotkey ID: %lli", wparam);
            break;
    }
}
