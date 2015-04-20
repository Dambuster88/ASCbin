#ifndef _ARGS_H_
#define _ARGS_H_

#include "stdio.h"

#define SEPARATORS_MAX	20
#define FORMAT_MAX		20

#define FORMAT_TYPE		0x70
#define SET_TYPE(x)		((x << 4) & FORMAT_TYPE)
#define GET_TYPE(x)		((x & FORMAT_TYPE) >> 4)

#define FORMAT_SIGN		0x04
#define SET_SIGN(x)		((x << 2) & FORMAT_SIGN)
#define GET_SIGN(x)		((x & FORMAT_SIGN) >> 2)

#define FORMAT_SIZE		0x03
#define SET_SIZE(x)		((x << 0) & FORMAT_SIZE)
#define GET_SIZE(x)		((x & FORMAT_SIZE) >> 0)

enum {
	HEX = 1,
	INT,
	FLOAT,
	ASCII
};

enum {
	UNSIGNED = 0,
	SIGNED = 1
};

enum {
	B1 = 0,
	B2 = 1,
	B3 = 2,
	B4 = 3
};

enum {
	ANYKIND = 1,
	ALL_HEX,
	ALL_DEC,
	ALL_FLOAT,
	ALL_ASCII
};

enum {
	LITTLE_ENDIAN = 1,
	BIG_ENDIAN
};

typedef union WordFormat_tag{
	uint8_t	byte;
	struct {
		uint8_t	Size:2;
		uint8_t	Sign:1;
		uint8_t	Type:4;
		uint8_t	:1;
	};
} WordFormat_t;

struct Options_tag {
	uint8_t	AllTheSame;
	uint8_t BigEndian;
	uint8_t Separator;
	uint8_t Format;
} Options;

extern char WORD_SEPARATORS[];
extern WordFormat_t FORMAT[];

uint32_t GetArgs (char argv[]);
uint32_t GetSeparators (char text[]);
uint32_t GetFormat (char text[]);

FILE* GetInput (char path[]);
FILE* GetOutput (void);

#endif
