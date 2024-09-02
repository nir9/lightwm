EXE_SRCS = wm.c tiling.c error.c keyboard.c shared_mem.c
DLL_SRCS = wm_dll.c error.c shared_mem.c

#all: release_gcc
all: debug release

debug: prep wm.c
	cl /DDEBUG /Zi /W3 $(EXE_SRCS) /link kernel32.lib user32.lib /out:debug/lightwm.exe /subsystem:windows /entry:wWinMain
	cl /DDEBUG /Zi /W3 $(DLL_SRCS) /LD /link kernel32.lib user32.lib /out:debug/lightwm_dll.dll /subsystem:windows /entry:DllMain

release: prep wm.c
	cl /Ox $(EXE_SRCS) /link kernel32.lib user32.lib /out:release/lightwm.exe /subsystem:windows /entry:wWinMain
	cl /Ox $(DLL_SRCS) /LD /link kernel32.lib user32.lib /out:release/lightwm_dll.dll /subsystem:windows /entry:DllMain

release_gcc: prep wm.c
	gcc -shared -o release/lightwm_dll.dll $(DLL_SRCS) -lkernel32 -luser32 -Wl,-eDllMain
	gcc -o release/lightwm.exe $(EXE_SRCS) -lkernel32 -luser32 -Wl,-ewWinMain -municode

prep:
	cmd /c IF NOT EXIST debug mkdir debug
	cmd /c IF NOT EXIST release mkdir release

clean:
	del *.obj *.exe *.dll *.lib *.exp *.ilk *.pdb *.res

	cmd /c IF EXIST debug del /S /Q debug
	cmd /c IF EXIST release del /S /Q release
	cmd /c IF EXIST debug rd /S /Q debug
	cmd /c IF EXIST release rd /S /Q release
