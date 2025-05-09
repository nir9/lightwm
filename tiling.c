#include "tiling.h"
#include "error.h"
#include <Windows.h>

#define MAX_MANAGED 1024

typedef struct {
	HWND handle;
	int workspaceNumber;
	bool shouldCleanup;
} ManagedWindow;

bool isFullscreen = false;
bool isTilingEnabled = true;
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

BOOL isWindowManagable(HWND windowHandle)
{
	if (!IsWindowVisible(windowHandle) || IsHungAppWindow(windowHandle)) {
		return FALSE;
	}

	WINDOWINFO winInfo;
	winInfo.cbSize = sizeof(WINDOWINFO);
	if (!GetWindowInfo(windowHandle, &winInfo)) {
		return FALSE;
	}

	if (winInfo.dwStyle & WS_POPUP) {
		return FALSE;
	}

	if (!(winInfo.dwExStyle & 0x20000000)) {
		return FALSE;
	}

	if (GetWindowTextLengthW(windowHandle) == 0) {
		return FALSE;
	}

	RECT clientRect;
	if (!GetClientRect(windowHandle, &clientRect)) {
		return FALSE;
	}

	// Skip small windows to avoid bugs
	if (clientRect.right < 100 || clientRect.bottom < 100){
		return FALSE;
	}

	return TRUE;
}

BOOL CALLBACK windowEnumeratorCallback(HWND currentWindowHandle, LPARAM lparam)
{
	if (numOfTotalManaged > MAX_MANAGED) {
		return FALSE;
	}

	if (!isWindowManagable(currentWindowHandle)) {
		return TRUE;
	}

	if (searchManaged(currentWindowHandle) != NULL) {
		return TRUE;
	}

	totalManaged[numOfTotalManaged].handle = currentWindowHandle;
	totalManaged[numOfTotalManaged].workspaceNumber = currentWorkspace;
	totalManaged[numOfTotalManaged].shouldCleanup = false;
	numOfTotalManaged++;

	return TRUE;
}

void updateManagedWindows()
{
	numOfCurrentlyManaged = 0;

	if (isFullscreen) {
		managed[0] = GetForegroundWindow();
		numOfCurrentlyManaged = 1;
		return;
	}

	for (int i = 0; i < numOfTotalManaged; i++) {
		if (totalManaged[i].workspaceNumber != currentWorkspace) {
			continue;
		}

		managed[numOfCurrentlyManaged] = totalManaged[i].handle;
		ShowWindow(managed[numOfCurrentlyManaged], SW_RESTORE);
		numOfCurrentlyManaged++;
	}
}

void tileWindows()
{
	if (!isTilingEnabled) {
		return;
	}

	if (newWorkspace) {
		newWorkspace = false;
	} else {
		if (!isFullscreen) {
			cleanupWorkspaceWindows();
		}
	}

	EnumChildWindows(GetDesktopWindow(), windowEnumeratorCallback, 0);

	if (numOfTotalManaged == 0) {
		return;
	}

	updateManagedWindows();

	TileWindows(GetDesktopWindow(), MDITILE_VERTICAL | MDITILE_SKIPDISABLED, NULL, numOfCurrentlyManaged, managed);
}

void toggleFullscreenMode()
{
	isFullscreen = !isFullscreen;
	newWorkspace = true;
	tileWindows();
}

void focusNextWindow(bool goBack, unsigned int callCount)
{
	// Avoid infinite recursion
	if (callCount > 25) {
		tileWindows();
		return;
	}

	if (isFullscreen) {
		toggleFullscreenMode();
	}

	currentFocusedWindowIndex += goBack ? -1 : 1;

	if (currentFocusedWindowIndex < 0) {
		currentFocusedWindowIndex = numOfCurrentlyManaged - 1;
	} else if (currentFocusedWindowIndex >= numOfCurrentlyManaged) {
		currentFocusedWindowIndex = 0;
	}

	if (!isWindowManagable(managed[currentFocusedWindowIndex]) || (GetForegroundWindow() == managed[currentFocusedWindowIndex])) {
		focusNextWindow(goBack, ++callCount);
	}

	SwitchToThisWindow(managed[currentFocusedWindowIndex], FALSE);
}

void gotoWorkspace(int number)
{
	if (isFullscreen) {
		isFullscreen = !isFullscreen;
		newWorkspace = true;
	}

	tileWindows();

	for (int i = 0; i < numOfCurrentlyManaged; i++) {
		CloseWindow(managed[i]);
	}

	currentWorkspace = number;
	newWorkspace = true;
	tileWindows();
}

void moveWindowToWorkspace(int workspaceNumber)
{
	if (numOfCurrentlyManaged == 0 || workspaceNumber == currentWorkspace) {
		return;
	}

	ManagedWindow* managedWindow = searchManaged(GetForegroundWindow());
	if (managedWindow == NULL) {
		return;
	}

	CloseWindow(managedWindow->handle);
	managedWindow->workspaceNumber = workspaceNumber;
	tileWindows();
}

void toggleDisableEnableTiling() {
	isTilingEnabled = !isTilingEnabled;

	if (isTilingEnabled) {
		tileWindows();
	}
}
