#include "shared_mem.h"
#include "debug.h"
#include "error.h"

HANDLE mapHandle = NULL;

bool readOrWriteToSharedMemory(DWORD write, DWORD* read) {
	bool succeeded = false;

	mapHandle = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(DWORD),
		"LightWMThreadId"
	);

	if (mapHandle == NULL) {
		reportWin32Error(L"Could not create file mapping object");
		goto cleanup;
	}

	LPVOID mapAddress = MapViewOfFile(
		mapHandle,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(DWORD)
	);

	if (mapAddress == NULL) {
		reportWin32Error(L"Could not map view of file");
		goto cleanup;
	}

	if (write) {
		*(DWORD*)mapAddress = write;
	} else if (read) {
		*read = *(DWORD*)mapAddress;
	}

	succeeded = true;

cleanup:
	if (mapAddress) {
		UnmapViewOfFile(mapAddress);
	}

	if (read) {
		cleanupMemoryMapFile();
	}

	return succeeded;
}

void cleanupMemoryMapFile() {
	if (mapHandle) {
		CloseHandle(mapHandle);
	}
}

bool retrieveDwordFromSharedMemory(DWORD* output) {
	return readOrWriteToSharedMemory(0, output);
}

bool storeDwordInSharedMemory(DWORD input) {
	return readOrWriteToSharedMemory(input, 0);
}
