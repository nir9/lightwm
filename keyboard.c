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

    return true;
}

void cleanupKeyboard()
{
    UnregisterHotKey(NULL, TOGGLE_FOCUS_MODE_HOYKEY_ID);
}

void handleHotkey(WPARAM wparam, LPARAM lparam) {
    switch (wparam) {
		case TOGGLE_FOCUS_MODE_HOYKEY_ID:
			toggleFocusedWindow(GetForegroundWindow());
			break;
        default:
            DEBUG_PRINT("Unhandled hotkey message! Hotkey ID: %lli", wparam);
            break;
    }
}
