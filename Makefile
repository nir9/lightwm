CC = cl
LD = link

EXEC = lightwm.exe
DLL = lightwm_dll.dll

all: clean_old $(DLL) $(EXEC)

$(EXEC): wm.c
	$(CC) wm.c tiling.c error.c user32.lib /link /out:$(EXEC)

$(DLL): wm_dll.obj
	$(LD) wm_dll.obj user32.lib /dll /out:$(DLL)

wm_dll.obj: wm_dll.c
	$(CC) /c wm_dll.c

# Temporary for a couple of weeks
clean_old:
	if exist wm.exe ( del wm.exe )
	if exist wm_dll.dll ( del wm_dll.dll )
	echo Old cleanup done

clean:
	del *.obj *.exe *.dll *.lib *.exp
