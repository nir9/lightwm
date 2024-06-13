#include "tiling.h"
#include "error.h"
#include <Windows.h>

#define MAX_MANAGED 1024

typedef struct {
	HWND handle;
	int workspaceNumber;
	bool isFocused;
	bool shouldCleanup;
} ManagedWindow;

HWND focusedWindow = 0;
HWND managed[MAX_MANAGED];
ManagedWindow totalManaged[MAX_MANAGED];
int numOfTotalManaged = 0;
int numOfCurrentlyManaged = 0;
int currentFocusedWindowIndex = 0;
int currentWorkspace = 1;
bool newWorkspace = false;

ManagedWindow* searchManaged(HWND handle)
{
	for (int i = 0; i < numOfTotalManaged; i++) {
		if (totalManaged[i].handle == handle) {
			return &totalManaged[i];
		}
	}

	return NULL;
}

void cleanupWorkspaceWindows()
{
	int keepCounter = 0;
	for (int i = 0; i < numOfTotalManaged; i++) {
		if (totalManaged[i].workspaceNumber == currentWorkspace) {
			continue;
		}

		totalManaged[keepCounter] = totalManaged[i];
		keepCounter++;
	}

	numOfTotalManaged = keepCounter;
}

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lparam)
{
	if (numOfTotalManaged > MAX_MANAGED) {
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

	if (searchManaged(hwnd) != NULL) {
		return TRUE;
	}

	totalManaged[numOfTotalManaged].handle = hwnd;
	totalManaged[numOfTotalManaged].isFocused = false;
	totalManaged[numOfTotalManaged].workspaceNumber = currentWorkspace;
	totalManaged[numOfTotalManaged].shouldCleanup = false;
	numOfTotalManaged++;

	return TRUE;
}

void updateManagedWindows()
{
	numOfCurrentlyManaged = 0;

	for (int i = 0; i < numOfTotalManaged; i++) {
		if (totalManaged[i].workspaceNumber != currentWorkspace) {
			continue;
		}

		if (totalManaged[i].isFocused) {
			managed[0] = totalManaged[i].handle;
			numOfCurrentlyManaged = 1;
			break;
		}

		managed[numOfCurrentlyManaged] = totalManaged[i].handle;
		ShowWindow(managed[numOfCurrentlyManaged], SW_RESTORE);
		numOfCurrentlyManaged++;
	}
}

void tileWindows()
{
	if (newWorkspace) {
		newWorkspace = false;
	} else {
		cleanupWorkspaceWindows();
	}

	EnumChildWindows(GetDesktopWindow(), EnumChildProc, 0);

	if (numOfTotalManaged == 0) {
		return;
	}

	updateManagedWindows();

	TileWindows(GetDesktopWindow(), MDITILE_VERTICAL | MDITILE_SKIPDISABLED, NULL, numOfCurrentlyManaged, managed);
}

void toggleFocusedWindow(HWND hwnd)
{
	if (focusedWindow != NULL) {
		searchManaged(focusedWindow)->isFocused = false;
		focusedWindow = NULL;
	} else {
		focusedWindow = hwnd;
		searchManaged(focusedWindow)->isFocused = true;
	}

	newWorkspace = true;
	tileWindows();
}

void focusNextWindow(bool goBack)
{
	if (focusedWindow != NULL) {
		toggleFocusedWindow(NULL);
	}

	currentFocusedWindowIndex += goBack ? -1 : 1;

	if (currentFocusedWindowIndex < 0) {
		currentFocusedWindowIndex = numOfCurrentlyManaged - 1;
	} else if (currentFocusedWindowIndex >= numOfCurrentlyManaged) {
		currentFocusedWindowIndex = 0;
	}

	SwitchToThisWindow(managed[currentFocusedWindowIndex], FALSE);
}

void gotoWorkspace(int number)
{
	for (int i = 0; i < numOfCurrentlyManaged; i++) {
		CloseWindow(managed[i]);
	}

	currentWorkspace = number;
	newWorkspace = true;
	tileWindows();
}
