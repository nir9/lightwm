#pragma once

#define _DEBUG

#ifdef _DEBUG
#define DEBUG_PRINT(format, ...) printf("DEBUG: Line %d: " format "\n", __LINE__, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...) 
#endif