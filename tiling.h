#pragma once

#include <Windows.h>
#include <stdbool.h>

void tileWindows();
void toggleFullscreenMode();
void focusNextWindow(bool, unsigned int);
void gotoWorkspace(int);
void moveWindowToWorkspace(int);
