#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type_defines.h"
#include "args.h"

uint32_t GetArgs (char argv[])
{
	uint32_t	i = 0;
	uint32_t	option = 1;
	uint32_t	WordLen = 0;
	
	WordLen = strlen(argv);
	if (WordLen == 0)
		return 0;

	if (argv[0] == '-' || argv[0] == '/') {
		argv = &argv[1];
		for (i = 0; i < WordLen - 1; i++) {
			switch (argv[i]) {
				case 'h':	// all hex
				if (Options.AllTheSame)
					return 0;
				Options.AllTheSame = ALL_HEX;
				printf ("All should be HEX!\r\n");
				break;
				
				case 'd':	// all decimal
				if (Options.AllTheSame)
					return 0;
				Options.AllTheSame = ALL_DEC;
				printf ("All should be Decimal!\r\n");
				break;
				
				case 'f':	// all float
				if (Options.AllTheSame)
					return 0;
				Options.AllTheSame = ALL_FLOAT;
				printf ("All should be Float!\r\n");
				break;
				
				case 'c':	//all ASCII
				if (Options.AllTheSame)
					return 0;
				Options.AllTheSame = ALL_ASCII;
				printf ("All should be ASCII!\r\n");
				break;
				
				case 'l':
				if (Options.BigEndian)
					return 0;
				Options.BigEndian = LITTLE_ENDIAN;
				printf ("Little Endian output will be used!\r\n");
				break;
				
				case 'b':
				if (Options.BigEndian)
					return 0;
				Options.BigEndian = BIG_ENDIAN;
				printf ("Big Endian output will be used!\r\n");
				break;
				
				case 'r':	//reverse
				printf ("Reverse!\r\n");
				return 0;
				break;
				
				default:
				printf ("ERROR: Unknown parameter: %c!\r\n", argv[i]);
				return 0;
			};
		};
	};
	
	if (Options.AllTheSame == 0) {
		Options.AllTheSame = ANYKIND;
	}
	
	if (Options.BigEndian == 0) {
		Options.BigEndian = LITTLE_ENDIAN;
		printf ("Little Endian output will be used, by default!\r\n");
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

