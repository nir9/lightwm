#pragma once

#include <Windows.h>
#include <stdbool.h>

void tileWindows();
void toggleFullscreenMode();
void focusNextWindow(bool goBack);
void gotoWorkspace(int number);
void moveWindowToWorkspace(int workspaceNumber);