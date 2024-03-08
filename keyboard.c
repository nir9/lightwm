#include <Windows.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include "keyboard.h"
#include "tiling.h"
#include "debug.h"
#include "error.h"

UINT getModifier(const char *value)
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

void addKeyboardKeybind(enum Action action, UINT modifier, UINT keyCode)
{
    if (!RegisterHotKey(NULL, action, modifier | MOD_NOREPEAT, keyCode)) {
        reportWin32Error(L"Failed to register hotkey");
		return;
    }

    DEBUG_PRINT("Registered %s hotkey", ACTION_STRINGS[action]);
}

bool bindKeyIfRelevant(ConfigItem configItem, enum Action action)
{
	if (configItem.name == NULL) {
		return false;
	}

	if (strcmp(configItem.name, ACTION_STRINGS[action]) == 0) {
		addKeyboardKeybind(
			action,
			getModifier(configItem.value),
			getKeyCode(configItem.value)
		);

		return true;
	}

	return false;
}

bool initializeKeyboardConfig(ConfigItems *configItems)
{
    for (size_t i = 1; i < configItems->configItemsCount; i++) {
		bool foundKey = false;

		for (size_t j = 0; j < TOTAL_ACTIONS_COUNT; j++) {
			if (bindKeyIfRelevant(configItems->configItem[i], (enum Action)j)) {
				foundKey = true;
				break;
			}
		}

		if (!foundKey) {
			wprintf(L"\nError: The lightwm.config configuration in APPDATA contains the action %S which is not supported by LightWM.\nYou can either delete the configuration in APPDATA and next time LightWM starts, it will create a new default one over there.\nOr you can take a look at the default config and understand the difference with your current config\n\n", configItems->configItem[i].name);
			return false;
		}
    }

    return true;
}

void cleanupKeyboard()
{
    UnregisterHotKey(NULL, WORKSPACE_1);
    UnregisterHotKey(NULL, WORKSPACE_2);
    UnregisterHotKey(NULL, WORKSPACE_3);
    UnregisterHotKey(NULL, WORKSPACE_4);
    UnregisterHotKey(NULL, NEXT_WINDOW);
    UnregisterHotKey(NULL, PREV_WINDOW);
    UnregisterHotKey(NULL, TOGGLE_FOCUS_MODE);
}

void handleHotkey(WPARAM wparam, LPARAM lparam) {
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
        case NEXT_WINDOW:
            DEBUG_PRINT("Highlight next window");
            break;
        case PREV_WINDOW:
            DEBUG_PRINT("Highlight previous window");
            break;
		case TOGGLE_FOCUS_MODE:
			toggleFocusedWindow(GetForegroundWindow());
			break;
        default:
            DEBUG_PRINT("Unhandled hotkey message! Hotkey ID: %lli", wparam);
            break;
    }
}
