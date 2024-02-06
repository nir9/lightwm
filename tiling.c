#include "tiling.h"
#include "error.h"
#include <Windows.h>

HWND focusedWindow = 0;
HWND managed[256];
int currentManagedIndex = 0;

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lparam) {
	if (currentManagedIndex > 255) {
		return FALSE;
	}

	if (!IsWindowVisible(hwnd) || IsHungAppWindow(hwnd)) {
		return TRUE;
	}

	WINDOWINFO winInfo;
	winInfo.cbSize = sizeof(WINDOWINFO);
	if (!GetWindowInfo(hwnd, &winInfo)) {
		return TRUE;
	}

	if (winInfo.dwStyle & WS_POPUP) {
		return TRUE;
	}

	if (GetWindowTextLengthW(hwnd) == 0) {
		return TRUE;
	}

	RECT clientRect;
	if (!GetClientRect(hwnd, &clientRect)) {
		return TRUE;
	}

	// Skip small windows to avoid bugs
	if (clientRect.right < 100 || clientRect.bottom < 100){
		return TRUE;
	}

	managed[currentManagedIndex] = hwnd;
	currentManagedIndex++;
	return TRUE;
}

void tileWindows() {
	currentManagedIndex = 0;

	if (focusedWindow == 0) {
		EnumChildWindows(GetDesktopWindow(), EnumChildProc, 0);

	} else {
		managed[currentManagedIndex] = focusedWindow;
		currentManagedIndex++;
	}

	if (currentManagedIndex == 0) {
		return;
	}

	TileWindows(GetDesktopWindow(), MDITILE_VERTICAL | MDITILE_SKIPDISABLED, NULL, currentManagedIndex, managed);
}

void toggleFocusedWindow(HWND hwnd) {
	if (focusedWindow != 0) {
		focusedWindow = 0;
	} else {
		focusedWindow = hwnd;
	}

	tileWindows();
}
