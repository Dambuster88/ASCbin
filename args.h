#ifndef _ARGS_H_
#define _ARGS_H_

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
} Options;

extern char WORD_SEPARATORS[];

uint32_t GetArgs (char argv[]);
uint32_t GetSeparators (char text[]);
FILE* GetInput (char path[]);
FILE* GetOutput (void);

#endif
