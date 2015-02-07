
COMPILER = gcc
DEFINES = 
FILES = ascbin.c args.c

make:
	$(COMPILER) -o ascbin $(FILES)

clean:
	del *.o
	del ascbin.exe