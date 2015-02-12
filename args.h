#ifndef _ARGS_H_
#define _ARGS_H_

#define SEPARATORS_MAX	20
#define FORMAT_MAX		20

#define FORMAT_TYPE		0x70
#define FORMAT_SIGN		0x04
#define FORMAT_SIZE		0x03

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
	B1 = 1,
	B2 = 2,
	B4 = 3,
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

struct Options_tag {
	uint8_t	AllTheSame;
	uint8_t BigEndian;
	uint8_t Separator;
	uint8_t Format;
} Options;

extern char WORD_SEPARATORS[];

uint32_t GetArgs (char argv[]);
uint32_t GetSeparators (char text[]);
uint32_t GetFormat (char text[]);

FILE* GetInput (char path[]);
FILE* GetOutput (void);

#endif
