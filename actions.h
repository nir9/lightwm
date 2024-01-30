#pragma once

//Just add the action to this preprocessor macro to generate the ENUM and string array
#define FOREACH_ACTION(ACTION) \
	ACTION(ACTION_NONE)  \
	ACTION(WORKSPACE_1)  \
	ACTION(WORKSPACE_2)  \
	ACTION(WORKSPACE_3)  \
	ACTION(WORKSPACE_4)  \
	ACTION(WINDOW_LEFT)  \
	ACTION(WINDOW_RIGHT) \
	ACTION(WINDOW_UP)    \
	ACTION(WINDOW_DOWN)


#define GENERATE_ENUM(ENUM) ENUM, 
#define GENERATE_STRINGS(STRING) #STRING, 

typedef enum
{ 
	FOREACH_ACTION(GENERATE_ENUM)
} Actions; 

static const char* ACTION_STRINGS[] = { 
	FOREACH_ACTION(GENERATE_STRINGS)
};