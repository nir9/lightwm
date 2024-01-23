#include <Windows.h>

HWND managed[256];
int currentManagedIndex = 0;

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lparam) {
    if (!IsWindowVisible(hwnd) || !IsHungAppWindow(hwnd)) {
        return TRUE;
    }

    if (currentManagedIndex > 255) {
        return FALSE;
    }

    managed[currentManagedIndex] = hwnd;
    currentManagedIndex++;
    return TRUE;
}

__declspec(dllexport) LRESULT CALLBACK ShellProc(int code, WPARAM wparam, LPARAM lparam) {

    if (code == HSHELL_WINDOWCREATED || code == HSHELL_WINDOWDESTROYED) {
        currentManagedIndex = 0;

        EnumChildWindows(GetDesktopWindow(), EnumChildProc, 0);

        TileWindows(GetDesktopWindow(), MDITILE_VERTICAL | MDITILE_SKIPDISABLED, NULL, currentManagedIndex, managed);
    }

    return CallNextHookEx(NULL, code, wparam, lparam);
}
