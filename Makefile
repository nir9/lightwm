# Define compiler and linker
CC = cl
LINKER = link
RC = rc

# Define common compiler flags
CFLAGS = /W3 /EHsc

# Define debug specific compiler flags
DBGCFLAGS = $(CFLAGS) /DDEBUG /Zi /Wall

# Define release specific compiler flags
RELCFLAGS = $(CFLAGS) /Ox

# Define source files
EXE_SRCS = wm.c tiling.c error.c config.c keyboard.c
DLL_SRCS = wm_dll.c error.c
RES_FILE = wm_resources.rc

# Define directories
DBGDIR = debug
RELDIR = release

# Define output names
EXE_NAME = lightwm.exe
DLL_NAME = lightwm_dll.dll

all: debug release

debug: clean prep resource wm.c
	$(CC) $(DBGCFLAGS) $(EXE_SRCS) $(RES_FILE).res /link user32.lib shell32.lib ole32.lib shlwapi.lib /out:$(DBGDIR)/$(EXE_NAME)
	$(CC) $(DBGCFLAGS) $(DLL_SRCS) /LD /link user32.lib /DEF:wm_dll.def /out:$(DBGDIR)/$(DLL_NAME)

release: clean prep resource wm.c
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

clean:
	del *.obj *.exe *.dll *.lib *.exp *.ilk *.pdb *.res

	@echo off > temp.bat && \
	echo IF EXIST $(DBGDIR) del /S /Q $(DBGDIR) >> temp.bat && \
	echo IF EXIST $(RELDIR) del /S /Q $(RELDIR) >> temp.bat && \
	echo IF EXIST $(DBGDIR) rd /S /Q $(DBGDIR) >> temp.bat && \
	echo IF EXIST $(RELDIR) rd /S /Q $(RELDIR) >> temp.bat && \
	temp.bat && \
	del temp.bat