#pragma once

#ifdef DEBUG
#define DEBUG_PRINT(format, ...) printf("DEBUG: %s Line %d: " format "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUG_WPRINT(format, ...) wprintf(L"DEBUG: %ls Line %d: " format "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...) 
#define DEBUG_WPRINT(format, ...)
#endif