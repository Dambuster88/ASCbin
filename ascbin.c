#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type_defines.h"
#include "args.h"

#define WORD_SEPARATORS " \n\r\t"

int main (int argc, char *argv[])
{
	FILE*	pInputFile;
	FILE*	pOutputFile;
	
	uint32_t	i = 0;
	
	uint32_t	inWords = 0;

	char	tmpStr[4096];
	char*	pWord;
	char*	result;

	if (argc == 3) {
		i = GetArgs(argv[1]);
		pInputFile = GetInput (argv[2]);
	} else {
		pInputFile = GetInput (argv[1]);
	}
	pOutputFile = GetOutput ();
	
	for (i = 0; i < 4096; i++) {
		tmpStr[i] = 0x00;
	}
	
	for (result = fgets(tmpStr, 4096, pInputFile); result; result = fgets(tmpStr, 4096, pInputFile)) {
		if (tmpStr != NULL) {
			for (pWord = strtok(tmpStr, WORD_SEPARATORS); pWord != NULL; pWord = strtok(NULL, WORD_SEPARATORS)) {
				inWords ++;
				printf ("%s", pWord);
				//getchar ();
			}
		} else {
			printf ("ERROR: Unknown error occurred at read!\r\n");
			fclose (pInputFile);
			fclose (pOutputFile);
			return 1;
		}
	}
	
	printf ("Input Words: %u\r\n", inWords);
	
	// Exit
	fclose (pInputFile);
	fclose (pOutputFile);
	
	printf ("Operation completed!\r\n");
	
	return 0;
}

