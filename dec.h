#ifndef _DEC_H_
#define _DEC_H_

uint32_t GetDec (char* str, FILE* output);
uint8_t	ASCIItoDEC1byte (char* str, uint8_t* result);
uint8_t	ASCIItoDEC2byte (char* str, uint16_t* result);
uint8_t	ASCIItoDEC4byte (char* str, uint32_t* result);

#endif
