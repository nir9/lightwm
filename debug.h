#pragma once

#ifdef DEBUG
#define DEBUG_PRINT(format, ...) printf("DEBUG: %s Line %d: " format "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...) 
#endif