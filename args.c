#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type_defines.h"
#include "args.h"


uint32_t GetArgs (char argv[])
{
	uint32_t	i = 0;
	uint32_t	option = 1;

	if (argv[0] == '-') {
		i = 1;
		switch (argv[i]) {
			case 'h':	// all hex
			option = 2;
			printf ("All HEX!\r\n");
			break;
			
			case 'd':	// all decimal
			option = 3;
			printf ("All Decimal!\r\n");
			break;
			
			case 'f':	// all float
			option = 4;
			printf ("All Float!\r\n");
			break;
			
			case 'r':	//reverse
			option = 5;
			printf ("Reverse!\r\n");
			break;
			
			default:
			printf ("ERROR: Unknown parameter: %c!\r\n", argv[i]);
			return 0;
		};
	};
	
	return option;
}

FILE* GetInput (char path[]) 
{
	FILE* inputFile;
	
	if (path) {
		inputFile = fopen(path, "r");
		if (inputFile == NULL) {
			printf ("ERROR: Could not open the input file: %s\r\n", path);
			return 0;
		} else {
			if (fgetc(inputFile) == EOF) {
				printf ("ERROR: The input file is empty!\r\n");
				fclose(inputFile);
				return 0;
			} else {
				rewind(inputFile);
			};
		};
	};
	
	return inputFile;
}

FILE* GetOutput (void) 
{
	const char path[] = "output.bin";
	FILE* outputFile;
	
	outputFile = fopen(path, "wb");

	if (outputFile == NULL) {
		printf ("ERROR: Could not open the output file: %s\r\n", path);
		return 0;
	};

	return outputFile;
}

