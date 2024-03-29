# LightWM
### Minimalist Tiling Window Manager For MS-Windows

## Usage

After building, run the executable ```lightwm.exe``` which will reside in the release and/or debug folder, depending on what you built.

Upon running, it will immediately tile all the non-minimized windows on your screen.

You can use ```alt+f``` hotkey to toggle focus mode which will disable tiling and put the focused window in fullscreen, pressing ```alt+f``` again will enable tiling again and tile all non-minimized windows

## Building

You can build using the "x64 Native Tools Command Prompt for VS 2022" that comes with Microsoft Visual Studio Build Tools:

In the console you can run nmake to build according to the Makefile

```nmake```

I haven't tried building with a different VS version or building x86, but that may work as well.

## Todo

- Add keyboard navigation between tiled windows
- Add support for Win10+ multiple desktops (CTRL + WinKey + Left/Right)

## Coding Conventions

#### Function and variable names

Functions and variables should be in camelCase

#### Comments

In general comments should be avoided unless they are absolutely necessary

Avoid adding obvious comments on the code like:
```c
// Prints hello on the screen
printf("Hello!\n");
```

Avoid comments explaining the logic in other words:
```c
// Allocate memory for the string and for the null terminator
malloc(1024 + 1);
```

Instead something like this is much better:
```c
const int nullTermSize = 1;
malloc(strlen(path) + nullTermSize);
```

Example of when a comment is necessary and has added value:
```c
// This DLL is loaded into many processes on the computer, therefore we need to keep the logic here as simple as possible to avoid slowing down the system
```

#### Pointers

```int*``` and not ```int *```

#### For/while/switch/if blocks

Opening braces { in the same line as the block definition

```c
if (cond) {
}
```

and not:

```c
if (cond)
{
}
```

#### Function blocks

Opening braces { in a new line after the function definition

```c
void init()
{
}
```

and not:

```c
void init() {
}
```

#### For/while/switch/if blocks

Space between the block name and the brackets

```if (statement)``` and not ```if(statement)```
