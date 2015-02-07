#include <stdio.h>
#include <stdlib.h>

typedef signed char		s8;
typedef unsigned char	u8;
typedef signed short	s16;
typedef unsigned short	u16;
typedef signed int		s32;
typedef unsigned int	u32;
typedef signed long		s64;
typedef unsigned long	u64;

typedef union {
	u8		byte[4];
	float	fl;
} float_t;

int main (int argc, char *argv[])
{
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

