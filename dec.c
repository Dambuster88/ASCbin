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
	
	uint8_t		unsig		= 1;
	
	uint8_t*	pWord;
	uint8_t*	chars;
	
	uint8_t		tmpChar;
	sint8_t		stmpChar;
	uint16_union_t	tmp16;
	sint16_union_t	stmp16;
	uint32_union_t	tmp32;
	sint32_union_t	stmp32;
	

	
	WordLen = strlen (str);
	if (WordLen <= 2)
		return 1;
	
	if (str[1] != ':') {
		if (str[1] == 's' && str[2] == ':' && WordLen > 3) {			// signed
			unsig = 0;
			pWord = &str[3];
		} else if (str[1] == 'u' && str[2] == ':' && WordLen > 3) {		// unsigned
			unsig = 1;
			pWord = &str[3];
			if (pWord[0] == '-')
				return 3;
		} else
			return 2;
	} else
		pWord = &str[2];
	
	switch (str[0] - '0') {
		case 1:
		if (unsig) {
			i = ASCIItoDEC_u1byte(pWord, &tmpChar);
			if (i)
				return 10+i;
			chars = &tmpChar;
		} else {
			i = ASCIItoDEC_s1byte(pWord, &stmpChar);
			if (i)
				return 10+i;
			chars = &stmpChar;
		}
		break;
		
		case 2:
		if (unsig) {
			i = ASCIItoDEC_u2byte(pWord, &tmp16.uint);
			if (i)
				return 10+i;
			chars = tmp16.byte;	
		} else {
			i = ASCIItoDEC_s2byte(pWord, &stmp16.sint);
			if (i)
				return 10+i;
			chars = stmp16.byte;
		}
		break;
		
		case 4:
		if (unsig) {
			i = ASCIItoDEC_u4byte(pWord, &tmp32.uint);
			if (i)
				return i;
			chars = tmp32.byte;
		} else {
			i = ASCIItoDEC_s4byte(pWord, &stmp32.sint);
			if (i)
				return i;
			chars = stmp32.byte;
		}
		break;
		
		default:
		printf ("ERROR: Wrong data size: %u!\r\n", str[0]);
		return 4;
	}
	
	for (i = 0; i < (str[0] - '0'); i++) {
		if (fputc (chars[i], output) != chars[i])
			return 20;
	}
		
	return 0;
}

uint8_t	ASCIItoDEC_u1byte (char* str, uint8_t* result)
{
	uint32_t	WordLen = 0;
	uint32_t	i		= 0;
	
	uint8_t		tmpResult = 0;
	uint8_t		lastResult = 0;
	
	WordLen = strlen(str);
	if (WordLen == 0 || WordLen > 3)
		return 1;

	for (i = 0; i < WordLen; i++) {
		if (str[i] >= '0' && str[i] <= '9') {
			tmpResult += (str[i] - '0') * power10(WordLen - i - 1);
			if (tmpResult < lastResult)		// overflow protection!
				return 3;
			lastResult = tmpResult;
		} else
			return 2;
	}
	
	*result = (uint8_t) tmpResult;
	
	return 0;
}

uint8_t	ASCIItoDEC_s1byte (char* str, sint8_t* result)
{
	uint32_t	WordLen = 0;
	uint32_t	i		= 0;
	
	uint8_t		negative	= 0;
	
	uint8_t	tmpResult	= 0x00;
	uint8_t	lastResult	= 0;

	if (str[0] == '-') {
		negative	= 1;
		str			= &str[1];
	} else if (str[0] == '+')
		str			= &str[1];
	else
		str			= &str[0];
	
	WordLen = strlen(str);
	if (WordLen == 0 || WordLen > 3)
		return 1;

	for (i = 0; i < WordLen; i++) {
		if (str[i] >= '0' && str[i] <= '9') {
			tmpResult += (str[i] - '0') * power10(WordLen - i - 1);
			if (tmpResult < lastResult)	// overflow protection!
				return 3;
			lastResult = tmpResult;
		} else
			return 2;
	}
	
	if (tmpResult > 0x7F + negative)
		return 4;
	
	if (negative)
		*result = (sint8_t) ((sint8_t) 0 - tmpResult);
	else
		*result = (sint8_t) ((sint8_t) 0 + tmpResult);
	
	return 0;
}

uint8_t	ASCIItoDEC_u2byte (char* str, uint16_t* result)
{
	uint32_t	WordLen = 0;
	uint32_t	i		= 0;
	
	uint16_t	tmpResult = 0;
	uint16_t	lastResult = 0;
	
	WordLen = strlen(str);
	if (WordLen == 0 || WordLen > 5)
		return 1;

	for (i = 0; i < WordLen; i++) {
		if (str[i] >= '0' && str[i] <= '9') {
			tmpResult += (str[i] - '0') * power10(WordLen - i - 1);
			if (tmpResult < lastResult)		// overflow protection!
				return 3;
			lastResult = tmpResult;
		} else
			return 2;
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
		negative	= 1;
		str			= &str[1];
	} else if (str[0] == '+')
		str			= &str[1];
	else
		str			= &str[0];
	
	WordLen = strlen(str);
	if (WordLen == 0 || WordLen > 5)
		return 1;

	for (i = 0; i < WordLen; i++) {
		if (str[i] >= '0' && str[i] <= '9') {
			tmpResult += (str[i] - '0') * power10(WordLen - i - 1);
			if (tmpResult < lastResult)		// overflow protection!
				return 3;
			lastResult = tmpResult;
		} else
			return 2;
	}
	
	if (tmpResult > 0x7FFF + negative)
		return 4;
	
	if (negative)
		*result = (sint16_t) ((sint16_t) 0 - tmpResult);
	else
		*result = (sint16_t) ((sint16_t) 0 + tmpResult);

	return 0;
}

uint8_t	ASCIItoDEC_u4byte (char* str, uint32_t* result)
{
	uint32_t	WordLen = 0;
	uint32_t	i		= 0;
	
	uint32_t	tmpResult = 0;
	uint32_t	lastResult = 0;
	
	WordLen = strlen(str);
	if (WordLen == 0 || WordLen > 10)
		return 1;

	for (i = 0; i < WordLen; i++) {
		if (str[i] >= '0' && str[i] <= '9') {
			tmpResult += (str[i] - '0') * power10(WordLen - i - 1);
			if (tmpResult < lastResult)		// overflow protection!
				return 3;
			lastResult = tmpResult;
		} else
			return 2;
	}
	
	*result = (uint32_t) tmpResult;
	
	return 0;
}

uint8_t	ASCIItoDEC_s4byte (char* str, sint32_t* result)
{
	uint32_t	WordLen = 0;
	uint32_t	i		= 0;
	
	uint8_t		negative	= 0;
	
	uint32_t	tmpResult = 0;
	uint32_t	lastResult = 0;

	if (str[0] == '-') {
		negative	= 1;
		str			= &str[1];
	} else if (str[0] == '+')
		str			= &str[1];
	else
		str			= &str[0];
	
	WordLen = strlen(str);
	if (WordLen == 0 || WordLen > 10)
		return 1;

	for (i = 0; i < WordLen; i++) {
		if (str[i] >= '0' && str[i] <= '9') {
			tmpResult += (str[i] - '0') * power10(WordLen - i - 1);
			if (tmpResult < lastResult)		// overflow protection!
				return 3;
			lastResult = tmpResult;
		} else
			return 2;
	}
	
	if (tmpResult > 0x7FFFFFFF + negative)
		return 4;
	
	if (negative)
		*result = (sint32_t) ((sint32_t) 0 - tmpResult);
	else
		*result = (sint32_t) ((sint32_t) 0 + tmpResult);
	
	return 0;
}

uint32_t power10 (uint8_t power)
{
	uint32_t	result = 1;
	uint32_t	i = 0;
	
	if (power > 10)
		return 0;
	
	for (i = 0; i < power; i++)
		result *= 10;
	
	return result;
}

