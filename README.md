# LightWM

### Minimalist Tiling Window Manager for Microsoft Windows

Written with C and Windows API and utilizes existing Windows facilities, resulting in very slim code.

Executable and DLL are very small (<20KB total)

Make sure you place the DLL next to the executable, so it can find it.

Video in which I make the PoC of this project: https://youtu.be/cuPirXZ6AWo

## Usage

After building, run the executable ```lightwm.exe``` which will reside in the release and/or debug folder, depending on what you built.

Upon running, it will immediately tile all the non-minimized windows on your screen. The program will run in the background.

If you want to configure it to run when you login, you can use the `Run` utility and type `shell:startup`, then create a LightWM shortcut in that folder.

### Hotkeys

- ```alt+q``` - quit LightWM
- ```alt+j``` - focus on next window
- ```alt+k``` - focus on previous window
- ```alt+f``` - toggle fullscreen mode (disables tiling and puts the focused window in fullscreen, pressing ```alt+f``` again will enable tiling again and tile all non-minimized windows)
- ```alt+1``` up until ```alt+9``` - go to workspace number ```1``` until ```9``` respectively
- You can move the current focused window to a different workspace number by adding ```shift``` to the above command.
- You can "force" retile by using ```alt+t```, you can use this as a trick to send the current focused window to the left.
- `alt+\` toggles whether the automatic tiling is enabled/disabled. This is useful if you also use programs which don't play well with LightWM automatic tiling

## Building

You can build using the "x64 Native Tools Command Prompt for VS 2022" that comes with Microsoft Visual Studio Build Tools:

In the console you can run nmake to build according to the Makefile

```nmake```

I haven't tried building with a different VS version or building x86, but that may work as well.

## Contributing

One of my main goals with this project is to keep it as minimal as possible, for example I decided against using a dynamic configuration on purpose.

Thus, make sure to keep the contributions as simple as possible.

If you plan on adding a completely new feature, let's discuss it on the issues beforehand.

Also notice that this project does not use any C library functions on purpose, only Windows API. This keeps everything as minimal as possible.

## Coding Conventions

#### Spacing

Tabs > Spaces

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

#### for/while/switch/if blocks

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

Space between the block name and the brackets

```if (statement)``` and not ```if(statement)```

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
