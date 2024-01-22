# LightWM
### Minimalist Tiling Window Manager For MS-Windows

## Building

You can build using the Microsoft Visual Studio Build Tools:

Making the main executable:

```cl wm.c user32.lib```

Making the DLL:

```cl /c wm_dll.c```

```link wm_dll.obj user32.lib /dll```

## Todo

- Add proper error handling and cleanup
- Add make file
- Add keyboard navigation
- Add workspaces
