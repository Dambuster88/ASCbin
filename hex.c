#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "type_defines.h"
#include "hex.h"

uint32_t GetHex (char* str, FILE* output)
{
	uint32_t	i			= 0;
	uint32_t	WordLen 	= 0;
	uint8_t		tmpChar		= 0x00;
	uint8_t		tmpCharR	= 0x00;
	
	WordLen = strlen (str);
	if (WordLen == 0 || WordLen % 2)
		return 0;
	
	for (i = 0; i < WordLen; i += 2) {
		if (CHARtoHEX(&str[i], &tmpChar))
			return 0;

		if (fputc (tmpChar, output) != tmpChar)
			return 0;
	}
	
	return i;
}

uint8_t	CHARtoHEX (char* str, uint8_t* result)
{
	uint32_t	WordLen = 0;
	uint32_t	i		= 0;
	
	if (strlen(str) == 0)
		return 1;
	
	if (str[0] >= '0' && str[0] <= '9') {
		*result = 16 * (str[0] - '0');
	} else if (str[0] >= 'A' && str[0] <= 'F') {
		*result = 16 * (str[0] - 'A' + 10);
	} else if (str[0] >= 'a' && str[0] <= 'f') {
		*result = 16 * (str[0] - 'a' + 10);
	} else {
		return 1;
	}
	
	if (str[1] >= '0' && str[1] <= '9') {
		*result += (str[1] - '0');
	} else if (str[1] >= 'A' && str[1] <= 'F') {
		*result += (str[1] - 'A' + 10);
	} else if (str[1] >= 'a' && str[1] <= 'f') {
		*result += (str[1] - 'a' + 10);
	} else {
		return 1;
	}
	
	return 0;
}
