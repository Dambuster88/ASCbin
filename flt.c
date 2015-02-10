#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "type_defines.h"
#include "flt.h"
#include "args.h"
#include "dec.h"

uint32_t GetFlt (char* str, uint8_t BigEndian, FILE* output)
{
	uint32_t	i = 0;
	uint32_t	WordLen = 0;
	char*		pWord;
	uint8_t*	chars;
	
	float_union_t flt;
	
	WordLen = strlen (str);
	
	if (str[0] == ':' && WordLen > 3) {
		pWord = &str[1];
	} else if (str[1] == ':' && WordLen > 4 && str[0] == '4') {
		pWord = &str[2];
	} else if (str[2] == ':' && WordLen > 5 && str[0] == '4' && str[1] == 'u') {
		pWord = &str[3];
	} else 
		return 1;

	flt.fl	= (float) atof (pWord);	// FIXME:	FIXIT:	TODO:	To be changed, it is rinsky!
	chars = flt.byte;
	
	if (BigEndian == BIG_ENDIAN) {
		for (i = 4; i > 0; i--)
			if (fputc(chars[i-1], output) != chars[i-1])
				return 21;
	} else if (BigEndian == LITTLE_ENDIAN) {
		for (i = 0; i < 4; i++)
			if (fputc(chars[i], output) != chars[i])
				return 22;
	} else
		return 20;
	
	
	return 0;
}

