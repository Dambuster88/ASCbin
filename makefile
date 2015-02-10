
COMPILER = gcc
DEFINES = 
FILES = ascbin.c args.c hex.c dec.c flt.c

make:
	$(COMPILER) -o ascbin $(FILES)

clean:
	del *.o
	del ascbin.exe