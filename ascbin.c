#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "type_defines.h"
#include "args.h"
#include "hex.h"
#include "flt.h"

int main (int argc, char *argv[])
{
	uint32_t	options	= 0;
	FILE*		pInputFile;
	FILE*		pOutputFile;
	
	uint32_t	i = 0;
	
	uint32_t	inLines = 0;
	uint32_t	inWords = 0;
	uint32_t	WordInLine = 0;

	char		tmpStr[4096];
	char*		pWord;
	uint32_t	WordLen = 0;
	
	char*		result;
	
	Options.AllTheSame	= 0;
	Options.BigEndian	= 0;
	Options.Separator	= 0;
	Options.Format		= 0;

	
	if (argc > 1) {
		if (argv[1][0] == '-' || argv[1][0] == '/') {
			i = GetArgs(argv[1]);
			if (i) {
				printf ("ERROR: Unexpected error: 01, %u", i);
				return 1;
			}
			
			if (Options.Separator)
				GetSeparators (argv[2]);
			
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
	
	for (i = 0; i < 4096; i++) {
		tmpStr[i] = 0x00;
	}
	
	for (result = fgets(tmpStr, 4096, pInputFile); result; result = fgets(tmpStr, 4096, pInputFile)) {
		inLines ++;
		WordInLine = 0;
		if (tmpStr != NULL) {
			for (pWord = strtok(tmpStr, WORD_SEPARATORS); pWord != NULL; pWord = strtok(NULL, WORD_SEPARATORS)) {
				if (pWord[0] == ';' || pWord[0] == '#')
					break;
				
				inWords ++;
				WordInLine ++;
				WordLen = strlen (pWord);
				
				if (Options.AllTheSame == ANYKIND)
					if (strchr(pWord, ':'))
						if (strchr(pWord, '.'))
							options = ALL_FLOAT;
						else
							options = ALL_DEC;
					else
						options = ALL_HEX;
				else
					options = Options.AllTheSame;
				
				switch (options) {
					case ALL_HEX:
					i = GetHex (pWord, Options.BigEndian, pOutputFile);
					if (i) {
						printf ("ERROR [%u]: Invalid HEX Word at line %u, word %u: %s\r\n", i, inLines, WordInLine, pWord);
						fclose (pInputFile);
						fclose (pOutputFile);
						return 1;
					};
					break;
					
					case ALL_DEC:
					i = GetDec (pWord, Options.BigEndian, pOutputFile);
					if (i) {
						printf ("ERROR [%u]: Invalid DEC Word at line %u, word %u: %s\r\n", i, inLines, WordInLine, pWord);
						fclose (pInputFile);
						fclose (pOutputFile);
					};
					break;
					
					case ALL_FLOAT:
					i = GetFlt (pWord, Options.BigEndian, pOutputFile);
					if (i) {
						printf ("ERROR [%u]: Invalid FLOAT Word at line %u, word %u: %s\r\n", i, inLines, WordInLine, pWord);
						fclose (pInputFile);
						fclose (pOutputFile);
					};
					break;
					
					case ALL_ASCII:
					printf ("ASCII chars not implemented yet!\r\n");
					return 0;
					
					default:
					printf ("ERROR: Unknown word type at line %u, word %u: %s\r\n", inLines, WordInLine, pWord);
					fclose (pInputFile);
					fclose (pOutputFile);
					return 1;
				}
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

