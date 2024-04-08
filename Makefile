EXE_SRCS = wm.c tiling.c error.c keyboard.c shared_mem.c
DLL_SRCS = wm_dll.c error.c shared_mem.c

DBGDIR = debug
RELDIR = release

all: clean_old debug release

debug: prep wm.c
	cl /DDEBUG /Zi /W3 $(EXE_SRCS) /link user32.lib /out:debug/lightwm.exe
	cl /DDEBUG /Zi /W3 $(DLL_SRCS) /LD /link user32.lib /out:debug/lightwm_dll.dll

release: prep wm.c
	$(CC) /Ox $(EXE_SRCS) /link user32.lib /out:release/lightwm.exe
	$(CC) /Ox $(DLL_SRCS) /LD /link user32.lib /out:release/lightwm_dll.dll

prep:
	@echo off > temp.bat && \
	echo IF NOT EXIST $(DBGDIR) mkdir $(DBGDIR) >> temp.bat && \
	echo IF NOT EXIST $(RELDIR) mkdir $(RELDIR) >> temp.bat && \
	temp.bat && \
	del temp.bat

# Temporary for a couple of weeks
clean_old:
	if exist wm.exe ( del wm.exe )
	if exist wm_dll.dll ( del wm_dll.dll )
	if exist lightwm.exe ( del lightwm.exe )
	if exist lightwm_dll.dll ( del lightwm_dll.dll )
	echo Old cleanup done

clean:
	del *.obj *.exe *.dll *.lib *.exp *.ilk *.pdb

	@echo off > temp.bat && \
	echo IF EXIST $(DBGDIR) del /S /Q $(DBGDIR) >> temp.bat && \
	echo IF EXIST $(RELDIR) del /S /Q $(RELDIR) >> temp.bat && \
	echo IF EXIST $(DBGDIR) rd /S /Q $(DBGDIR) >> temp.bat && \
	echo IF EXIST $(RELDIR) rd /S /Q $(RELDIR) >> temp.bat && \
	temp.bat && \
	del temp.bat
