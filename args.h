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
} Options;

uint32_t GetArgs (char argv[]);
FILE* GetInput (char path[]);
FILE* GetOutput (void);

#endif
