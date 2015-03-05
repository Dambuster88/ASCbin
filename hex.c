#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "type_defines.h"
#include "hex.h"
#include "args.h"

uint32_t CheckHex (char* str, char** OutStr)
{
	uint32_t	WordLen 	= 0;
	uint8_t		result = 0;
	
	WordLen = strlen (str);
	if (WordLen == 0 || WordLen % 2 || WordLen > 8)
		return 0;
	
	*OutStr = &str[0];
	
	if (WordLen == 8)
		result = B4;
	else if (WordLen == 6)
		result = B3;
	else if (WordLen == 4)
		result = B2;
	else if (WordLen == 2)
		result = B1;
	else 
		return 0;
	
	return SET_TYPE(HEX) | SET_SIZE(result);
}

uint32_t ASCIItoHEX (char* str, char* result)
{
	uint32_t	i = 0;
	uint32_t	WordLen = 0;
	char*		chars;
	
	WordLen = strlen (str);
	if (WordLen == 0)
		return 1;
	
	for (i = 0; i < WordLen/2; i++) {
		if (CHARtoHEX(&str[i*2], &result[WordLen/2 - i - 1])) // Write Little Endian, from the end to the start!
			return 2;
	}
	
	return 0;
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
