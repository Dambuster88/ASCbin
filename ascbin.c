#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "type_defines.h"
#include "args.h"
#include "hex.h"
#include "flt.h"

#define WORD_SEPARATORS " \n\r\t"


int main (int argc, char *argv[])
{
	uint32_t	options = 0;
	FILE*	pInputFile;
	FILE*	pOutputFile;
	
	uint32_t	i = 0;
	
	uint32_t	inLines = 0;
	uint32_t	inWords = 0;
	uint32_t	WordInLine = 0;

	char	tmpStr[4096];
	char*	pWord;
	uint32_t	WordLen = 0;
	
	char*	result;
	
	Options.AllTheSame	= 0;
	Options.BigEndian	= 0;

	if (argc == 3) {
		options = GetArgs(argv[1]);
		pInputFile = GetInput (argv[2]);
	} else {
		pInputFile = GetInput (argv[1]);
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
				if (pWord[0] == ';' || pWord[0] == '#') {
					break;
				}
				inWords ++;
				WordInLine ++;
				WordLen = strlen (pWord);
				
				if (Options.AllTheSame == ANYKIND) {
					// TODO
				} else
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
					printf ("Not implemented yet!\r\n");
					return 0;
					
					default:
					printf ("ERROR: Unknown word type at line %u, word %u: %s\r\n", inLines, WordInLine, pWord);
					fclose (pInputFile);
					fclose (pOutputFile);
					return 1;
				}
				//printf ("%s", pWord);
				//getchar ();
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

