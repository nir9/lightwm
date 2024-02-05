#include <Windows.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>

#include "keyboard.h"

#include "debug.h"

UINT getModifier(const char *value);

UINT getKeyCode(const char *value);

void addKeyboardKeybind(enum Action action, UINT modifier, UINT keyCode);

void bindKeyIfRelevant(ConfigItem configItem, enum Action action) {
	if (configItem.name == NULL) {
		return;
	}

	if (strcmp(configItem.name, ACTION_STRINGS[action]) == 0) {
		addKeyboardKeybind(
			action,
			getModifier(configItem.value),
			getKeyCode(configItem.value)
		);
	}
}

BOOL initializeKeyboardConfig(const ConfigItems *configItems) {
    assert(configItems != NULL);
    assert(configItems->configItem != NULL);
    assert(configItems->configItemsCount != 0);

    for (size_t i = 1; i <= configItems->configItemsCount; i++) {
        bindKeyIfRelevant(configItems->configItem[i], WORKSPACE_1);
        bindKeyIfRelevant(configItems->configItem[i], WORKSPACE_2);
        bindKeyIfRelevant(configItems->configItem[i], WORKSPACE_3);
        bindKeyIfRelevant(configItems->configItem[i], WORKSPACE_4);
        bindKeyIfRelevant(configItems->configItem[i], WINDOW_UP);
        bindKeyIfRelevant(configItems->configItem[i], WINDOW_DOWN);
        bindKeyIfRelevant(configItems->configItem[i], WINDOW_LEFT);
        bindKeyIfRelevant(configItems->configItem[i], WINDOW_RIGHT);
    }

    return TRUE;
}

void cleanupKeyboard() {
    UnregisterHotKey(NULL, WORKSPACE_1);
    UnregisterHotKey(NULL, WORKSPACE_2);
    UnregisterHotKey(NULL, WORKSPACE_3);
    UnregisterHotKey(NULL, WORKSPACE_4);
    UnregisterHotKey(NULL, WINDOW_UP);
    UnregisterHotKey(NULL, WINDOW_DOWN);
    UnregisterHotKey(NULL, WINDOW_LEFT);
    UnregisterHotKey(NULL, WINDOW_RIGHT);
    DEBUG_PRINT("Unregistered all hotkeys");
}

UINT getModifier(const char *value) {
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
    return MOD_ALT; //TODO Throw an error here
}

UINT getKeyCode(const char *value) {
    DEBUG_PRINT("GetKeyCode char value '%c'", value[strlen(value) - 1]);
    return VkKeyScanEx(value[strlen(value) - 1], GetKeyboardLayout(0));
}

void addKeyboardKeybind(enum Action action, UINT modifier, UINT keyCode) {
    if (!RegisterHotKey(NULL, action, modifier | MOD_NOREPEAT, keyCode)) {
        if (GetLastError() == ERROR_HOTKEY_ALREADY_REGISTERED) {
            puts("Warn: Hotkey already registerd\n");
            return;
        }

        MessageBox(NULL, "Failed to register hotkey.", "Error", MB_OK | MB_ICONERROR);
    }

    DEBUG_PRINT("Registered %s hotkey", ACTION_STRINGS[action]);
}

LRESULT handleHotkey(WPARAM wparam, LPARAM lparam) {
    switch (wparam) {
        case WORKSPACE_1:
            DEBUG_PRINT("Switch to workspace 1");
            break;
        case WORKSPACE_2:
            DEBUG_PRINT("Switch to workspace 2");
            break;
        case WORKSPACE_3:
            DEBUG_PRINT("Switch to workspace 3");
            break;
        case WORKSPACE_4:
            DEBUG_PRINT("Switch to workspace 4");
            break;
        case WINDOW_UP:
            DEBUG_PRINT("Highlight window above");
            break;
        case WINDOW_DOWN:
            DEBUG_PRINT("Highlight window below");
            break;
        case WINDOW_LEFT:
            DEBUG_PRINT("Highlight window left");
            break;
        case WINDOW_RIGHT:
            DEBUG_PRINT("Highlight window right");
            break;
        default:
            DEBUG_PRINT("Unhandled hotkey message! Hotkey ID: %lli", wparam);
            break;
    }

    return ERROR_SUCCESS;
}
