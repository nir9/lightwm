#include "tiling.h"
#include "error.h"
#include "debug.h"
#include <Windows.h>

HWND focusedWindow = 0;
HWND managed[256];
int currentManagedIndex = 0;
int currentFocusedManaged = 0;

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lparam)
{
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

	if (!(winInfo.dwExStyle & 0x20000000)) {
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

	// temp
	char theTitle[256] = {0};
	GetWindowTextA(hwnd, theTitle, 256);
	DEBUG_PRINT("title: %s\n", theTitle);
	DEBUG_PRINT("style: %x\n", winInfo.dwExStyle);

	managed[currentManagedIndex] = hwnd;
	currentManagedIndex++;
	return TRUE;
}

void tileWindows()
{
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

void toggleFocusedWindow(HWND hwnd)
{
	if (focusedWindow != 0) {
		focusedWindow = 0;
	} else {
		focusedWindow = hwnd;
	}

	tileWindows();
}

void focusNextWindow(bool goBack)
{
	currentFocusedManaged += goBack ? -1 : 1;

	if (currentFocusedManaged < 0) {
		currentFocusedManaged = currentManagedIndex - 1;
	}

	if (managed[currentFocusedManaged] == 0) {
		currentFocusedManaged = 0;
	}

	SwitchToThisWindow(managed[currentFocusedManaged], FALSE);
}
