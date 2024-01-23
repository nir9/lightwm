#include <windows.h>
#include "shortcuts.h"

void SpawnTerminal() { // ALT + Q -> Spawn a Terminal Window
    if (RegisterHotKey(NULL, NULL, 0x0001, 0x51)) {
        
    }
} 

void KillWindow() { // ALT + C -> Kill focused Window
    
    HWND hActiveWindow = GetActiveWindow();
    if (hActiveWindow = NULL) {
        wprintf(L"[!] Error getting handle to active window\n, Error: %ld", GetLastError());
    }
    
    if (RegisterHotkey(NULL, NULL, 0x0001, 0x43)) {
        wprintf(L"[+] Killing Currently Active Window\n");
        CloseWindow(hActiveWindow);
    }
} 