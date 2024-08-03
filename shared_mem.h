#pragma once

#include <Windows.h>
#include <stdbool.h>

bool retrieveDwordFromSharedMemory(DWORD* output);
bool storeDwordInSharedMemory(DWORD input);
void cleanupMemoryMapFile();
