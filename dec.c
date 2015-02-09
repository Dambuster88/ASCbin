#include <string.h>
#include <stdio.h>
#include <math.h>
//#include <stdlib.h>
#include "type_defines.h"
#include "dec.h"

uint32_t power10 (uint8_t power);

uint32_t GetDec (char* str, FILE* output)
{
	uint32_t	i			= 0;
	uint32_t	WordLen 	= 0;
	
	uint8_t		negativ		= 0;
	uint8_t		unsig		= 1;
	
	uint8_t		nStart		= 2;
	
	uint8_t		tmpChar;
	sint8_t		stmpChar;
	uint16_union_t	tmp16;
	sint16_union_t	stmp16;
	uint32_union_t	tmp32;
	sint32_union_t	stmp32;
	uint8_t		tmpCharR	= 0x00;
	
	uint8_t*	chars;
	
	WordLen = strlen (str);
	if (WordLen <= 2)
		return 1;
	
	if (str[1] != ':') {
		if (str[1] == 's' && str[2] == ':' && WordLen > 3) {			// signed + 
			unsig = 0;
			nStart = 3;
			if (str[nStart] == '-' && WordLen > 4) {					// signed -
				negativ = 1;
			};
		} else if (str[1] == 'u' && str[2] == ':' && WordLen > 3) {		// unsigned
			unsig = 1;
			nStart = 3;
			if (str[nStart] == '-') {
				return 3;
			}
		} else {
			return 2;
		}
	}
	
	switch (str[0] - '0') {
		case 1:
		if (unsig) {
			if (ASCIItoDEC_u1byte(&str[nStart], &tmpChar))
				return 10;
			chars = &tmpChar;
		} else {
			if (ASCIItoDEC_s1byte(&str[nStart], &stmpChar))
				return 11;
			chars = &stmpChar;
		}
		break;
		
		case 2:
		if (ASCIItoDEC_u2byte(&str[nStart], &tmp16.uint))
			return 20;
		chars = tmp16.byte;
		break;
		
		case 4:
		if (unsig) {
			if (ASCIItoDEC_u4byte(&str[nStart], &tmp32.uint))
				return 40;
			chars = tmp32.byte;
		} else {
			if (ASCIItoDEC_s4byte(&str[nStart], &stmp32.sint))
				return 41;
			chars = stmp32.byte;
		}
		break;
		
		default:
		printf ("ERROR: Wrong data size: %u!\r\n", str[0]);
		return 4;
	}
	
	for (i = 0; i < (str[0] - '0'); i++) {
		if (fputc (chars[i], output) != chars[i])
			return 50;
	}
		
	return 0;
}

uint8_t	ASCIItoDEC_u1byte (char* str, uint8_t* result)
{
	uint32_t	WordLen = 0;
	uint32_t	i		= 0;
	uint32_t	tmpResult = 0;
	
	WordLen = strlen(str);
	if (WordLen == 0 || WordLen > 3)
		return 1;

	for (i = 0; i < WordLen; i++) {
		if (str[i] >= '0' && str[i] <= '9') {
			tmpResult += (str[i] - '0') * power10(WordLen - i - 1);
			if (tmpResult > 0xFF) {
				return 3;
			};
		} else {
			return 2;
		}
	}
	
	*result = (uint8_t) tmpResult;
	
	return 0;
}

uint8_t	ASCIItoDEC_s1byte (char* str, sint8_t* result)
{
	uint32_t	WordLen = 0;
	uint32_t	i		= 0;
	
	uint8_t		negative	= 0;
	uint8_t		signal		= 0;
	
	uint8_t	tmpResult = 0;
	uint8_t	lastResult = 0;

	if (str[0] == '-') {
		negative = 1;
		signal = 1;
		WordLen = strlen(&str[1]);
		str = &str[1];
	} else if (str[0] == '+') {
		signal = 1;
		WordLen = strlen(&str[1]);
		str = &str[1];
	} else {
		WordLen = strlen(str);
	}
	
	if (WordLen == 0 || WordLen > 3)
		return 1;

	for (i = 0; i < WordLen; i++) {
		if (str[i] >= '0' && str[i] <= '9') {
			tmpResult += (str[i] - '0') * power10(WordLen - i - 1);
			if (tmpResult < lastResult) {		// overflow protection!
				return 3;
			};
			lastResult = tmpResult;
		} else {
			return 2;
		}
	}
	
	if (tmpResult > 0x7F + negative) {
		return 4;
	};
	
	if (negative)
		tmpResult = 0x80 | tmpResult;

	*result = (sint8_t) tmpResult;
	
	return 0;
}

uint8_t	ASCIItoDEC_u2byte (char* str, uint16_t* result)
{
	uint32_t	WordLen = 0;
	uint32_t	i		= 0;
	uint32_t	tmpResult = 0;
	
	WordLen = strlen(str);
	if (WordLen == 0 || WordLen > 5)
		return 1;

	for (i = 0; i < WordLen; i++) {
		if (str[i] >= '0' && str[i] <= '9') {
			tmpResult += (str[i] - '0') * power10(WordLen - i - 1);
			if (tmpResult > 0xFFFF) {
				return 3;
			};
		} else {
			return 2;
		}
	}
	
	*result = (uint16_t) tmpResult;
	
	return 0;
}

uint8_t	ASCIItoDEC_s2byte (char* str, sint16_t* result)
{
	uint32_t	WordLen = 0;
	uint32_t	i		= 0;
	
	uint8_t		negative	= 0;
	uint8_t		signal		= 0;
	
	uint16_t	tmpResult = 0;
	uint16_t	lastResult = 0;

	if (str[0] == '-') {
		negative = 1;
		signal = 1;
		WordLen = strlen(&str[1]);
		str = &str[1];
	} else if (str[0] == '+') {
		signal = 1;
		WordLen = strlen(&str[1]);
		str = &str[1];
	} else {
		WordLen = strlen(str);
	}
	
	if (WordLen == 0 || WordLen > 5)
		return 1;

	for (i = 0; i < WordLen; i++) {
		if (str[i] >= '0' && str[i] <= '9') {
			tmpResult += (str[i] - '0') * power10(WordLen - i - 1);
			if (tmpResult < lastResult) {		// overflow protection!
				return 3;
			};
			lastResult = tmpResult;
		} else {
			return 2;
		}
	}
	
	if (tmpResult > 0x7FFF + negative) {
		return 4;
	};
	
	if (negative)
		tmpResult = 0x8000 | tmpResult;

	*result = (sint8_t) tmpResult;
	
	return 0;
}

uint8_t	ASCIItoDEC_u4byte (char* str, uint32_t* result)
{
	uint32_t	WordLen = 0;
	uint32_t	i		= 0;
	uint32_t	tmpResult = 0;
	
	WordLen = strlen(str);
	if (WordLen == 0 || WordLen > 10)
		return 1;

	for (i = 0; i < WordLen; i++) {
		if (str[i] >= '0' && str[i] <= '9') {
			tmpResult += (str[i] - '0') * power10(WordLen - i - 1);
			if (tmpResult > 0xFFFFFFFF) {
				return 3;
			};
		} else {
			return 2;
		}
	}
	
	*result = (uint32_t) tmpResult;
	
	return 0;
}

uint8_t	ASCIItoDEC_s4byte (char* str, sint32_t* result)
{
	uint32_t	WordLen = 0;
	uint32_t	i		= 0;
	
	uint8_t		negative	= 0;
	uint8_t		signal		= 0;
	
	uint32_t	tmpResult = 0;
	uint32_t	lastResult = 0;

	if (str[0] == '-') {
		negative = 1;
		signal = 1;
		WordLen = strlen(&str[1]);
		str = &str[1];
	} else if (str[0] == '+') {
		signal = 1;
		WordLen = strlen(&str[1]);
		str = &str[1];
	} else {
		WordLen = strlen(str);
	}
	
	if (WordLen == 0 || WordLen > 10)
		return 1;

	for (i = 0; i < WordLen; i++) {
		if (str[i] >= '0' && str[i] <= '9') {
			tmpResult += (str[i] - '0') * power10(WordLen - i - 1);
			if (tmpResult < lastResult) {		// overflow protection!
				return 3;
			};
			lastResult = tmpResult;
		} else {
			return 2;
		}
	}
	
	if (tmpResult > 0x7FFFFFFF + negative) {
		return 4;
	};
	
	if (negative)
		tmpResult = 0x80000000 | tmpResult;

	*result = (sint32_t) tmpResult;
	
	return 0;
}

uint32_t power10 (uint8_t power)
{
	uint32_t	result = 1;
	uint32_t	i = 0;
	
	if (power > 10)
		return 0;
	
	for (i = 0; i < power; i++) {
		result *= 10;
	}
	
	return result;
}

