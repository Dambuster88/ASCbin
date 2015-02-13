#include <string.h>
#include <stdio.h>
#include <math.h>
//#include <stdlib.h>
#include "type_defines.h"
#include "dec.h"
#include "args.h"

uint32_t CheckInt (char* str, char** OutStr)
{
	uint32_t	i			= 0;
	uint32_t	WordLen 	= 0;
	
	uint8_t		sig			= 0;
	
	uint8_t*	pWord;
	uint8_t*	chars;
	
	uint8_t		tmpChar;
	sint8_t		stmpChar;
	uint16_union_t	tmp16;
	sint16_union_t	stmp16;
	uint32_union_t	tmp32;
	sint32_union_t	stmp32;
	
	uint8_t	result;
	
	WordLen = strlen (str);
	if (WordLen <= 2)
		return 1;
	
	if (str[1] != ':') {
		if (str[1] == 's' && str[2] == ':' && WordLen > 3) {			// signed
			sig = 1;
			pWord = &str[3];
		} else if (str[1] == 'u' && str[2] == ':' && WordLen > 3) {		// unsigned
			sig = 0;
			pWord = &str[3];
			if (pWord[0] == '-')
				return 3;
		} else
			return 2;
	} else
		pWord = &str[2];
	
	if (str[0] == '4')
		result = B4;
	else if (str[0] == '2')
		result = B2;
	else if (str[0] == '1')
		result = B1;
	else
		return 4;
	
	*OutStr = pWord;

	return SET_TYPE(INT) | SET_SIGN(sig) | SET_SIZE(result);
}

uint8_t	ASCIItoDEC_u1byte (char* str, char* result)
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
	
	result[0] = tmpResult;
	
	return 0;
}

uint8_t	ASCIItoDEC_s1byte (char* str, char* result)
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
		tmpResult = ((sint8_t) 0 - tmpResult);
	else
		tmpResult = ((sint8_t) 0 + tmpResult);
	
	result[0] = tmpResult;
	
	return 0;
}

uint8_t	ASCIItoDEC_u2byte (char* str, char* result)
{
	uint32_t	WordLen = 0;
	uint32_t	i		= 0;

	uint16_t		lastResult = 0;	
	uint16_union_t	tmpResult;
	tmpResult.uint	= 0;
	
	WordLen = strlen(str);
	if (WordLen == 0 || WordLen > 5)
		return 1;

	for (i = 0; i < WordLen; i++) {
		if (str[i] >= '0' && str[i] <= '9') {
			tmpResult.uint += (str[i] - '0') * power10(WordLen - i - 1);
			if (tmpResult.uint < lastResult)		// overflow protection!
				return 3;
			lastResult = tmpResult.uint;
		} else
			return 2;
	}
	
	for (i = 0; i < 2; i++)
		result[i] = tmpResult.byte[i];
	
	return 0;
}

uint8_t	ASCIItoDEC_s2byte (char* str, char* result)
{
	uint32_t	WordLen = 0;
	uint32_t	i		= 0;
	
	uint8_t		negative	= 0;
	uint8_t		signal		= 0;
	
	uint16_t	lastResult = 0;
	uint16_t	tmpResult = 0;
	sint16_union_t	Result;

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
		Result.sint = (sint16_t) ((sint16_t) 0 - tmpResult);
	else
		Result.sint = (sint16_t) ((sint16_t) 0 + tmpResult);
	
	for (i = 0; i < 2; i++)
		result[i] = Result.byte[i];

	return 0;
}

uint8_t	ASCIItoDEC_u4byte (char* str, char* result)
{
	uint32_t	WordLen = 0;
	uint32_t	i		= 0;
	
	uint32_t		lastResult = 0;
	uint32_union_t	tmpResult;
	tmpResult.uint = 0;
	
	WordLen = strlen(str);
	if (WordLen == 0 || WordLen > 10)
		return 1;

	for (i = 0; i < WordLen; i++) {
		if (str[i] >= '0' && str[i] <= '9') {
			tmpResult.uint += (str[i] - '0') * power10(WordLen - i - 1);
			if (tmpResult.uint < lastResult)		// overflow protection!
				return 3;
			lastResult = tmpResult.uint;
		} else
			return 20 + i;
	}
	
	for (i = 0; i < 4; i++)
		result[i] = tmpResult.byte[i];
	
	return 0;
}

uint8_t	ASCIItoDEC_s4byte (char* str, char* result)
{
	uint32_t	WordLen = 0;
	uint32_t	i		= 0;
	
	uint8_t		negative	= 0;
	
	uint32_t	tmpResult = 0;
	uint32_t	lastResult = 0;
	sint32_union_t	Result;

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
		Result.sint = (sint32_t) ((sint32_t) 0 - tmpResult);
	else
		Result.sint = (sint32_t) ((sint32_t) 0 + tmpResult);
	
	for (i = 0; i < 4; i++)
		result[i] = Result.byte[i];
	
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

