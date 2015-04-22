#include <string.h>
#include "type_defines.h"
#include "args.h"
#include "format.h"

#define		DEBUG	0

///
///	Variables
///


///
///	Functions
///

///
uint8_t GetFormat_Word (char* pWord)
{
	uint32_t	WordLen;
	WordFormat_t result;
	
	result.byte = 0;

#if (DEBUG == 1)
	printf ("%s\r\n", pWord);
#endif

	WordLen = strlen (pWord);
	if (WordLen == 0)
		return 0;
	
	switch (pWord[0]) {
		case 'h':
		switch (pWord[1]) {
			case '1':
			result.Size = B1;
			break;
			
			case '2':
			result.Size = B2;
			break;
			
			case '4':
			result.Size = B4;
			break;
			
			default:
			printf ("ERROR: Unknown word size:%s!\r\n", pWord);
			return 0;
		}
		result.Type = HEX;
		break;
		
		case 'i':
		switch (pWord[1]) {
			case '1':
			result.Size = B1;
			break;
			
			case '2':
			result.Size = B2;
			break;
			
			case '4':
			result.Size = B4;
			break;
			
			default:
			printf ("ERROR: Unknown word size:%s!\r\n", pWord);
			return 0;
		}
		switch (pWord[2]) {
			case 's':
			result.Sign = SIGNED;
			break;
			
			case 'u':
			result.Sign = UNSIGNED;
			break;

			default:
			printf ("ERROR: Unknown word size:%s!\r\n", pWord);
			return 0;
		}
		result.Type = INT;
		break;
		
		case 'f':
		result.Type = FLOAT;
		result.Size = B4;
		break;
		
		case 'a':
		result.Type = ASCII;
		result.Size = B1;
		break;
		
		default:
		printf ("ERROR: Unknown word format:%s!\r\n", pWord);
		return 0;
	}
	
#if (DEBUG == 1)
	printf ("%02X   ", result.byte);
	printf ("%d %d %d\r\n", result.Type, result.Sign, result.Size);
#endif

	return result.byte;
}
