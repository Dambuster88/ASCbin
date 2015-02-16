#ifndef	_TYPE_DEFINES_H_
#define _TYPE_DEFINES_H_

typedef signed char		sint8_t;
typedef unsigned char	uint8_t;
typedef signed short	sint16_t;
typedef unsigned short	uint16_t;
typedef signed int		sint32_t;
typedef unsigned int	uint32_t;
typedef signed long		sint64_t;
typedef unsigned long	uint64_t;

typedef union {
	uint8_t		byte[2];
	sint16_t	sint;
} sint16_union_t;

typedef union {
	uint8_t		byte[2];
	uint16_t	uint;
} uint16_union_t;

typedef union {
	uint8_t		byte[4];
	sint32_t	sint;
} sint32_union_t;

typedef union {
	uint8_t		byte[4];
	uint32_t	uint;
} uint32_union_t;

typedef union {
	uint8_t		byte[4];
	sint64_t	sint;
} sint64_union_t;

typedef union {
	uint8_t		byte[8];
	uint64_t	uint;
} uint64_union_t;

typedef union {
	uint8_t		byte[4];
	float		fl;
} float_union_t;

#endif
