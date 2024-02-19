# LightWM
### Minimalist Tiling Window Manager For MS-Windows

## Building

You can build using the "x64 Native Tools Command Prompt for VS 2022" that comes with Microsoft Visual Studio Build Tools:

In the console you can run nmake to build according to the Makefile

```nmake```

I haven't tried building with a different VS version or building x86, but that may work as well.

## Todo

- Add keyboard navigation
- Add workspaces

## Coding Conventions

- Function and variable names: camelCase
- Comments: Avoid adding obvious comments on the code and use them only if they have any added value
- Pointers: ```int*``` and not ```int *```
- For/while/switch/if blocks: opening braces { in the same line as the block definition ( e.g. ```if {``` and not ```if\n{``` )
- Function blocks: opening braces { in a new line after the function definition ( e.g. ```void init()\n{``` and not ```void init() {``` )
- For/while/switch/if blocks: space between the block name and the brackets ( e.g. ```if (statement)``` and not ```if(statement)``` )
