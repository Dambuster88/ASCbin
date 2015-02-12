#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type_defines.h"
#include "args.h"

char WORD_SEPARATORS [SEPARATORS_MAX]	= " \t\r\n";
char FORMAT[FORMAT_MAX];

uint32_t GetArgs (char argv[])
{
	uint32_t	i = 0;
	uint32_t	WordLen = 0;
	
	WordLen = strlen(argv);
	if (WordLen == 0)
		return 1;

	if (argv[0] == '-' || argv[0] == '/') {
		argv = &argv[1];
		for (i = 0; i < WordLen - 1; i++) {
			switch (argv[i]) {
				case 'h':	// all hex
				if (Options.AllTheSame)
					return 21;
				Options.AllTheSame = ALL_HEX;
				printf ("All should be HEX!\r\n");
				break;
				
				case 'd':	// all decimal
				if (Options.AllTheSame)
					return 22;
				Options.AllTheSame = ALL_DEC;
				printf ("All should be Decimal!\r\n");
				break;
				
				case 'f':	// all float
				if (Options.AllTheSame)
					return 23;
				Options.AllTheSame = ALL_FLOAT;
				printf ("All should be Float!\r\n");
				break;
				
				case 'c':	//all ASCII
				if (Options.AllTheSame)
					return 24;
				Options.AllTheSame = ALL_ASCII;
				printf ("All should be ASCII!\r\n");
				break;
				
				case 'l':
				if (Options.BigEndian)
					return 25;
				Options.BigEndian = LITTLE_ENDIAN;
				printf ("Little Endian output will be used!\r\n");
				break;
				
				case 'b':
				if (Options.BigEndian)
					return 26;
				Options.BigEndian = BIG_ENDIAN;
				printf ("Big Endian output will be used!\r\n");
				break;
				
				case 's':
				if (Options.Separator)
					return 27;
				Options.Separator = 1;
				break;
				
				case 'F':
				if (Options.Format)
					return 28;
				Options.Format = 1;
				break;
				
				case 'r':	//reverse
				printf ("Reverse!\r\n");
				return 28;
				break;
				
				default:
				printf ("ERROR: Unknown parameter: %c!\r\n", argv[i]);
				return 20;
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
	
	return 0;
}

uint32_t GetSeparators (char text[])
{
	uint32_t	i = 0;
	uint32_t	j = 0;
	uint32_t	WordLen = 0;
	uint32_t	index = 0;
	char		new_separator;
	
	index = strlen (WORD_SEPARATORS);
	WordLen = strlen (text);
	
	for (i = 0; i < WordLen && index < 20; i++) {
		if (text[i] == '\\') {
			switch (text[i+1]) {
				case '\'':
				new_separator = 0x27;
				break;
				
				case '\"':
				new_separator = 0x22;
				break;
				
				case '\?':
				new_separator = 0x3F;
				break;
				
				case '\\':
				new_separator = 0x5C;
				break;
				
				case 'a':
				new_separator = 0x07;
				break;
				
				case 'b':
				new_separator = 0x08;
				break;
				
				case 'f':
				new_separator = 0x0C;
				break;
				
				case 'n':
				new_separator = 0x0A;
				break;
				
				case 'r':
				new_separator = 0x0D;
				break;
				
				case 't':
				new_separator = 0x09;
				break;
				
				case 'v':
				new_separator = 0x0B;
				break;
				
				case '0' ... '9':
				printf ("ERROR: Not supported character format: \\nnn\r\n");
				return 1;
				
				case 'x':
				if ((text[i+2] >= '0' && text[i+2] <= '9')) {
					new_separator = (char)(16 * (text[i+2] - '0'));
				} else if ((text[i+2] >= 'a' && text[i+2] <= 'f')) {
					new_separator = (char)(16 * (text[i+2] - 'a' + 10));
				} else if ((text[i+2] >= 'A' && text[i+2] <= 'F')) {
					new_separator = (char)(16 * (text[i+2] - 'A' + 10));
				} else {
					printf ("ERROR: Invalid separator format: \\x%c%c\r\n", text[i+2], text[i+3]);
				}

				if ((text[i+3] >= '0' && text[i+3] <= '9')) {
					new_separator += (char) (text[i+3] - '0');
				} else if ((text[i+3] >= 'a' && text[i+3] <= 'f')) {
					new_separator += (char) (text[i+3] - 'a' + 10);
				} else if ((text[i+3] >= 'A' && text[i+3] <= 'F')) {
					new_separator += (char) (text[i+3] - 'A' + 10);
				} else {
					printf ("ERROR: Invalid separator format: \\x%c%c\r\n", text[i+2], text[i+3]);
				}
				
				i += 2;
				break;
				
				case 'u':
				printf ("ERROR: Not supported character format: \\unnnn\r\n");
				return 1;
				
				case 'U':
				printf ("ERROR: Not supported character format: \\Unnnnnnnn\r\n");
				return 1;
				
				default:
				printf ("ERROR: Unknown escape sequence: \\%c\r\n", text[i+1]);
				return 1;
			}
			i++;
		} else {
			new_separator = text[i];
		}
		
		if (!strchr(WORD_SEPARATORS, new_separator)) {
			WORD_SEPARATORS[index] = new_separator;
			index++;
		}
		// else {nothing to do!}
	}
}

uint32_t GetFormat (char text[])
{
	uint32_t	index = 0;
	uint32_t	i = 0;
	
	char*		pWord;
	char*		pWordLen;
	uint32_t	WordLen;
	
	uint32_t	StrLen = strlen(text);
	
	if (StrLen == 0)
		return 1;
	
	for (i = 0; i < FORMAT_MAX; i++)
		FORMAT[i] = 0x00;
	
	for (pWord = strtok(text, " "); pWord != NULL; pWord = strtok(NULL, " ")) {
		if (index == FORMAT_MAX) {
			printf ("ERROR: Format argument is too long!\r\n");
			return 20;
		}
		
		WordLen = strlen (pWord);
		if (WordLen == 0 || WordLen > 3)
			return 21;
		
		switch (pWord[0]) {
			case 'h':
			if (WordLen == 2)
				FORMAT[index] = ((HEX << 4) & FORMAT_TYPE);
			else 
				return 31;
			break;
			
			case 'i':
			if (WordLen == 2 || WordLen == 3)
				FORMAT[index] = ((INT << 4) & FORMAT_TYPE);
			else
				return 32;
			break;
			
			case 'f':
			if (WordLen == 1)
				FORMAT[index] = ((FLOAT << 4) & FORMAT_TYPE);
			else
				return 33;
			break;
			
			case 'a':
			if (WordLen == 1)
				FORMAT[index] = ((ASCII << 4) & FORMAT_TYPE);
			else
				return 34;
			break;
			
			default:
			printf ("ERROR: Unknown word format:%s!\r\n", pWord);
			return 30;
		}
		
		if (pWord[0] == 'h' || pWord[0] == 'i')
			switch (pWord[1]) {
				case '1':
				FORMAT[index] |= (B1 & FORMAT_SIZE);
				break;
				
				case '2':
				FORMAT[index] |= (B2 & FORMAT_SIZE);
				break;
				
				case '4':
				FORMAT[index] |= (B4 & FORMAT_SIZE);
				break;
				
				default:
				printf ("ERROR: Unknown word length:%s\r\n", pWord);
				return 22;
			}
		
		if (pWord[0] == 'i')
			if (WordLen == 3)
				switch (pWord[2]) {
					case 's':
					FORMAT[index] |= ((SIGNED << 2) & FORMAT_SIGN);
					break;
					
					case 'u':
					FORMAT[index] |= ((UNSIGNED << 2) & FORMAT_SIGN);
					break;
					
					default:
					printf ("ERROR: Unknown signes of the word:%s\r\n", pWord);
					return 23;
				}
			else
				FORMAT[index] |= ((UNSIGNED << 2) & FORMAT_SIGN);
		
		index++;
	}
	
#if 0
	WordLen = strlen (FORMAT);
	printf ("FORMAT [%u]:", WordLen);
	for (i = 0; i < WordLen; i++)
		printf ("%02X\t", (char)FORMAT[i]);
	printf ("\r\n");
#endif	

	return 0;
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

