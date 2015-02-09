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
	uint8_t		tmpChar		= 0x00;
	uint16_union_t	tmp16;
	uint32_union_t	tmp32;
	uint8_t		tmpCharR	= 0x00;
	
	WordLen = strlen (str);
	if (WordLen <= 2 || str[1] != ':')
		return 0;
	
	if (str[2] == '-')
		negativ = 1;
	
	switch (str[0] - '0') {
		case 1:
		if (ASCIItoDEC1byte(&str[2], &tmpChar))
			return 0;
		
		if (fputc (tmpChar, output) != tmpChar)
			return 0;
		i++;
		break;
		
		case 2:
		if (ASCIItoDEC2byte(&str[2], &tmp16.uint))
			return 0;
		
		if (fputc (tmp16.byte[0], output) != tmp16.byte[0])
			return 0;
		i++;
		if (fputc (tmp16.byte[1], output) != tmp16.byte[1])
			return 0;
		i++;
		break;
		
		case 4:
		if (ASCIItoDEC4byte(&str[2], &tmp32.uint))
			return 0;
		
		if (fputc (tmp32.byte[0], output) != tmp32.byte[0])
			return 0;
		i++;
		if (fputc (tmp32.byte[1], output) != tmp32.byte[1])
			return 0;
		i++;
		if (fputc (tmp32.byte[2], output) != tmp32.byte[2])
			return 0;
		i++;
		if (fputc (tmp32.byte[3], output) != tmp32.byte[3])
			return 0;
		i++;
		break;
		
		default:
		printf ("ERROR: Wrong data size: %u!\r\n", str[0]);
		return 0;
	}
	
	return i;
}

uint8_t	ASCIItoDEC1byte (char* str, uint8_t* result)
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

uint8_t	ASCIItoDEC2byte (char* str, uint16_t* result)
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

uint8_t	ASCIItoDEC4byte (char* str, uint32_t* result)
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

