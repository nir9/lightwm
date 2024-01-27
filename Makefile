CC = cl
LD = link
RC = rc 

EXEC = wm.exe
DLL = wm_dll.dll
RESOURCES=wm_dll_resources.obj

# Boiler plate
CFLAGS  = /EHsc
LDFLAGS = 

EXE_SRCS = wm.c tiling.c error.c
DLL_SRCS = wm_dll.c error.c config.c keyboard.c

EXE_OBJS = $(EXE_SRCS:.c=.obj)
DLL_OBJS = $(DLL_SRCS:.c=.obj)

all: $(DLL) $(EXEC)

$(EXEC): $(EXE_OBJS) 
	$(CC) $(CFLAGS) /Fe:$@ $^ /link user32.lib

$(DLL): $(DLL_OBJS) $(RESOURCES)
	$(CC) $(CFLAGS) /LD /Fe:$@ $^ /link user32.lib shell32.lib ole32.lib shlwapi.lib /DEF:wm_dll.def

%.obj: %.c
	$(CC) $(CFLAGS) /c /Fo:$@ $<

%.obj: %.rc
	$(RC) /fo $@ $<

clean:
	del *.obj $(DLL) $(EXEC) *.lib *.exp
