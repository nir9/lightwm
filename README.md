# LightWM

### Minimalist Tiling Window Manager for Microsoft Windows

Video in which I make the PoC of this project: https://youtu.be/cuPirXZ6AWo

## Usage

After building, run the executable ```lightwm.exe``` which will reside in the release and/or debug folder, depending on what you built.

Upon running, it will immediately tile all the non-minimized windows on your screen.

The program will run in the background. To quit LightWM, use ```alt+q```.

You can use ```alt+j``` and ```alt+k``` to go to the next/previous window with your keyboard.

You can use ```alt+f``` hotkey to toggle focus mode which will disable tiling and put the focused window in fullscreen, pressing ```alt+f``` again will enable tiling again and tile all non-minimized windows

You can use ```alt+1``` and ```alt+2``` and ```alt+3``` to go to workspace number 1, 2 and 3 respectively

## Building

You can build using the "x64 Native Tools Command Prompt for VS 2022" that comes with Microsoft Visual Studio Build Tools:

In the console you can run nmake to build according to the Makefile

```nmake```

I haven't tried building with a different VS version or building x86, but that may work as well.

## Contributing

One of my main goals with this project is to keep it as minimal as possible, for example I decidd against using a dynamic configuration on purpose. Thus, make sure to keep the contributions as simple as possible. If you plan on adding a completely new feature, let's discuss it on the issues beforehand.

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

Opening braces ```{``` in the same line as the block definition

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

Opening braces ```{``` in a new line after the function definition

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
