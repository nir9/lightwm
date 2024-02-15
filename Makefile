CC = cl
LINKER = link
RC = rc

CFLAGS = /W3

DBGCFLAGS = $(CFLAGS) /DDEBUG /Zi /W3 # temporarly not Wall

RELCFLAGS = $(CFLAGS) /Ox

EXE_SRCS = wm.c tiling.c error.c config.c keyboard.c shared_mem.c
DLL_SRCS = wm_dll.c error.c shared_mem.c
RES_FILE = wm_resources.rc

DBGDIR = debug
RELDIR = release

EXE_NAME = lightwm.exe
DLL_NAME = lightwm_dll.dll

all: clean_old debug release

debug: prep resource wm.c
	$(CC) $(DBGCFLAGS) $(EXE_SRCS) $(RES_FILE).res /link user32.lib shell32.lib ole32.lib shlwapi.lib /out:$(DBGDIR)/$(EXE_NAME)
	$(CC) $(DBGCFLAGS) $(DLL_SRCS) /LD /link user32.lib /DEF:wm_dll.def /out:$(DBGDIR)/$(DLL_NAME)

release: prep resource wm.c
	$(CC) $(RELCFLAGS) $(EXE_SRCS) $(RES_FILE).res /link user32.lib shell32.lib ole32.lib shlwapi.lib /out:$(RELDIR)/$(EXE_NAME)
	$(CC) $(RELCFLAGS) $(DLL_SRCS) /LD /link user32.lib /DEF:wm_dll.def /out:$(RELDIR)/$(DLL_NAME)

resource: $(RES_FILE)
	$(RC) /fo $(RES_FILE).res $(RES_FILE)

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
	del *.obj *.exe *.dll *.lib *.exp *.ilk *.pdb *.res

	@echo off > temp.bat && \
	echo IF EXIST $(DBGDIR) del /S /Q $(DBGDIR) >> temp.bat && \
	echo IF EXIST $(RELDIR) del /S /Q $(RELDIR) >> temp.bat && \
	echo IF EXIST $(DBGDIR) rd /S /Q $(DBGDIR) >> temp.bat && \
	echo IF EXIST $(RELDIR) rd /S /Q $(RELDIR) >> temp.bat && \
	temp.bat && \
	del temp.bat
