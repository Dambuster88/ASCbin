#include <stdio.h>
#include <stdlib.h>
#include "type_defines.h"

typedef union {
	u8		byte[4];
	float	fl;
} float_t;

int main (int argc, char *argv[])
{
	char 	FilePath[MAX_PATH];

	FILE*	pInputFile;
	FILE*	pOutputFile;
	
	
	u32			i		= 0;
	double 		dbl		= 0;
	float_t		flv;
	flv.fl	= 0.0;
	
	printf ("%s + %u\r\n", argv[0], argc-1);
	
	for (i = 1; i < argc; i++) {
		dbl = atof(argv[i]);
		flv.fl = (float) dbl;
		flv.fl = (float) atof(argv[i]);
		printf ("%s\r\n", argv[i]);
		printf ("%02X %02X %02X %02X\r\n\n", flv.byte[3], flv.byte[2], flv.byte[1], flv.byte[0]);
	}
	
	return 0;
}

