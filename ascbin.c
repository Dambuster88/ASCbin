#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "type_defines.h"
#include "args.h"
#include "hex.h"
#include "flt.h"
#include "version.h"

/* definition to expand macro then apply to pragma message */
#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var " = "  VALUE(var)

#if (!defined SHA1 && (VER_SW_MAJOR == 0 && VER_SW_MINOR == 0))
	#error "No version detected!"
#endif

uint32_t WriteEndian (uint8_t str[], uint32_t len, uint8_t direction, FILE* output);

int main (int argc, char *argv[])
{
	uint32_t	options	= 0;
	FILE*		pInputFile;
	FILE*		pOutputFile;
	
	uint32_t	i = 0;
	
	uint32_t	inLines = 0;
	uint32_t	inWords = 0;
	uint32_t	WordInLine = 0;

	char		LineStr[4096];
	char*		pWord;
	char*		pNumber;
	char		pStr[4];
	uint32_t	WordLen = 0;
	uint8_t		WordType;
	
	uint32_t	FormatLen = 0;
	uint32_t	FormatIndex = 0;
	
	char*		result;
	
	Options.AllTheSame	= 0;
	Options.BigEndian	= 0;
	Options.Separator	= 0;
	Options.Format		= 0;

	printf ("Version: %u.%u.%u\r\n", VER_SW_MAJOR, VER_SW_MINOR, VER_SW_REVISION, SHA1);
	
	if (argc > 1) {
		if (argv[1][0] == '-' || argv[1][0] == '/') {
			i = GetArgs(argv[1]);
			if (i) {
				printf ("ERROR: Unexpected error: 01, %u", i);
				return 1;
			}
			
			if (Options.Separator)
				if (GetSeparators (argv[2])) {
					printf ("ERROR: Invalid separator!\r\n");
					return 1;
				}
			
			if (Options.Format)
				if (Options.Separator)
					i = GetFormat (argv[3]);
				else
					i = GetFormat (argv[2]);

			if (i) {
				printf ("ERROR[%u]: Invalid Line Format!\r\n", i);
				return 1;
			}
				
			i = argc - 2 - Options.Separator - Options.Format;
			if (i)
				pInputFile = GetInput (argv[argc - 1]);
			else
				pInputFile = GetInput ("input.txt");
		} else {
			// no options, should be only the input file
			Options.AllTheSame	= ANYKIND;
			Options.BigEndian	= LITTLE_ENDIAN;
			pInputFile = GetInput (argv[1]);
		}
	} else {
		// no arguments, input.txt file is used by default
		Options.AllTheSame	= ANYKIND;
		Options.BigEndian	= LITTLE_ENDIAN;
		pInputFile = GetInput ("input.txt");
	}

	pOutputFile = GetOutput ();
	if (pOutputFile == NULL) {
		fclose (pInputFile);
		return 1;
	}
	
	for (i = 0; i < 4096; i++)
		LineStr[i] = 0x00;
	
	for (i = 0; i < 4; i++)
		pStr[i] = 0x00;
	
	for (result = fgets(LineStr, 4096, pInputFile); result; result = fgets(LineStr, 4096, pInputFile)) {
		inLines ++;
		WordInLine = 0;
		
		FormatIndex = 0;
		
		if (LineStr != NULL) {
			for (pWord = strtok(LineStr, WORD_SEPARATORS); pWord != NULL; pWord = strtok(NULL, WORD_SEPARATORS)) {
				if (pWord[0] == ';' || pWord[0] == '#')
					break;
				
				inWords ++;
				WordInLine ++;
				WordLen = strlen (pWord);
				if (WordLen == 0)
					return 1;
				
				if (Options.Format) {
					FormatLen = strlen (FORMAT);
					if (FormatLen > FORMAT_MAX)
						return 1;
					
					WordType = FORMAT[FormatIndex];
					if (WordType == 0x00) {
						printf ("ERROR: Line %u is bigger than the format string\r\n", inLines);
						return 1;
					}
					pNumber = pWord;
				} else {
					if (Options.AllTheSame == ANYKIND) {
						if (strchr(pWord, ':')) {
							if (strchr(pWord, '.')) {
								WordType = CheckFloat (pWord, &pNumber);
								
							} else {
								WordType = CheckInt (pWord, &pNumber);
							}
						} else {
							WordType = CheckHex (pWord, &pNumber);
						}
					} else {
						WordType = Options.AllTheSame;
					}
				}
				
				if (WordType == 0) {
					printf ("ERROR: Word not detected well!\r\n");
					return 1;
				}
				
				switch (GET_TYPE(WordType)) {
					case HEX:
					i = ASCIItoHEX(pWord, pStr);
					if (i) {
						printf ("ERROR [%u]: Invalid HEX Word at line %u, word %u: %s\r\n", i, inLines, WordInLine, pWord);
						fclose (pInputFile);
						fclose (pOutputFile);
						return 1;
					};
					
					if (GET_SIZE(WordType) == B1)
						WordLen = 1;
					else if (GET_SIZE(WordType) == B2)
						WordLen = 2;
					else if (GET_SIZE(WordType) == B3)
						WordLen = 3;
					else if (GET_SIZE(WordType) == B4)
						WordLen = 4;
					else
						return 1;
					break;
					
					case INT:
					if (GET_SIZE(WordType) == B1)
						if (GET_SIGN(WordType) == UNSIGNED) {
							i = ASCIItoDEC_u1byte (pNumber, pStr);
							WordLen = 1;
						} else if (GET_SIGN(WordType) == SIGNED) {
							i = ASCIItoDEC_s1byte (pNumber, pStr);
							WordLen = 1;
						} else
							return 1;
					else if (GET_SIZE(WordType) == B2)
						if (GET_SIGN(WordType) == UNSIGNED) {
							i = ASCIItoDEC_u2byte (pNumber, pStr);
							WordLen = 2;
						} else if (GET_SIGN(WordType) == SIGNED) {
							i = ASCIItoDEC_s2byte (pNumber, pStr);
							WordLen = 2;
						} else
							return 1;
					else if (GET_SIZE(WordType) == B4)
						if (GET_SIGN(WordType) == UNSIGNED) {
							i = ASCIItoDEC_u4byte (pNumber, pStr);
							WordLen = 4;
						} else if (GET_SIGN(WordType) == SIGNED) {
							i = ASCIItoDEC_s4byte (pNumber, pStr);
							WordLen = 4;
						} else
							return 1;
					else
						return 1;
						
					if (i) {
						printf ("ERROR [%u]: Invalid DEC Word at line %u, word %u: %s\r\n", i, inLines, WordInLine, pWord);
						fclose (pInputFile);
						fclose (pOutputFile);
					};
					break;
					
					case FLOAT:
					i = ASCIItoFLOAT (pNumber, pStr);
					if (i == 0x00) {
						printf ("ERROR [%u]: Invalid FLOAT Word at line %u, word %u: %s\r\n", i, inLines, WordInLine, pWord);
						fclose (pInputFile);
						fclose (pOutputFile);
						return 1;
					};
					WordLen = 4;
					break;
					
					case ASCII:
					printf ("ASCII chars not implemented yet!\r\n");
					return 0;
					
					default:
					printf ("ERROR: Unknown word type at line %u, word %u: %s\r\n", inLines, WordInLine, pWord);
					fclose (pInputFile);
					fclose (pOutputFile);
					return 1;
				}
				
				i = WriteEndian (pStr, WordLen, Options.BigEndian, pOutputFile);
				if (i) {
					printf ("ERROR [%u]: Can not write the following: %02X %02X %02X %02X \r\n", i, pStr[3], pStr[2], pStr[1], pStr[0]);
					fclose (pInputFile);
					fclose (pOutputFile);
					return 1;
				};
				
				FormatIndex ++;
			}
		} else {
			printf ("ERROR: Unknown error occurred at read!\r\n");
			fclose (pInputFile);
			fclose (pOutputFile);
			return 1;
		}
	}
	
	printf ("Input Lines: %u\r\n", inLines);
	printf ("Input Words: %u\r\n", inWords);
	
	// Exit
	fclose (pInputFile);
	fclose (pOutputFile);
	
	printf ("Operation completed!\r\n");
	
	return 0;
}

uint32_t WriteEndian (uint8_t str[], uint32_t len, uint8_t direction, FILE* output)
{
	uint32_t	i = 0;
	if (len > 4)
		return 1;
	
	if (direction == BIG_ENDIAN) {
		for (i = len; i > 0; i--)
			if (fputc(str[i-1], output) != str[i-1])
				return 30+i;
	} else if (direction == LITTLE_ENDIAN) {
		for (i = 0; i < len; i++)
			if (fputc(str[i], output) != str[i])
				return 40+i;
	} else
		return 20;
	
	for (i = 0; i < 4; i++)
		str[i] = 0x00;

	return 0;
}

