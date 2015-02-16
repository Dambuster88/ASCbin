#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "type_defines.h"
#include "flt.h"
#include "args.h"
#include "dec.h"

uint32_t CheckFloat (char* str, char** OutStr)
{
	uint32_t	i = 0;
	uint32_t	WordLen = 0;
	char*		pWord;
	uint8_t*	chars;
	
	float_union_t flt;
	
	WordLen = strlen (str);
	
	if (str[0] == ':' && WordLen > 3) {
		*OutStr = &str[1];
	} else if (str[1] == ':' && WordLen > 4 && str[0] == '4') {
		*OutStr = &str[2];
	} else 
		return 0;
	
	return SET_TYPE(FLOAT);
}

uint32_t ASCIItoFLOAT (char* str, char* result)
{
	uint32_t		i = 0;
	float_union_t	flt;
	
	// TODO: Check the range!!!
	
	flt.fl	= (float) atof (str);	// FIXME:	FIXIT:	TODO:	To be changed, it is risky!
	
	for (i = 0; i < 4; i++) {
		result[i] = flt.byte[i];
	}
	
	return 1;
}

