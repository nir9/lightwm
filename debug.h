#pragma once

#ifdef DEBUG

#include <stdio.h>
#include <stdarg.h>

inline void DebugPrintHelper(const char* format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsprintf_s(buffer, 1024, format, args);
    va_end(args);
    OutputDebugStringA(buffer);
}

inline void DebugWPrintHelper(const wchar_t* format, ...) {
    wchar_t buffer[1024];
    va_list args;
    va_start(args, format);
    vswprintf_s(buffer, 1024, format, args);
    va_end(args);
    OutputDebugStringW(buffer);
}

#define DEBUG_PRINT(format, ...) printf("DEBUG: %s Line %d: " format "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
	DebugPrintHelper(format, ##__VA_ARGS__)
#define DEBUG_WPRINT(format, ...) wprintf(L"DEBUG: %ls Line %d: " format "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
	DebugWPrintHelper(format, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...) 
#define DEBUG_WPRINT(format, ...)
#endif