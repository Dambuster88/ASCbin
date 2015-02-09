#ifndef _DEC_H_
#define _DEC_H_

uint32_t GetDec (char* str, FILE* output);

uint8_t	ASCIItoDEC_u1byte (char* str, uint8_t* result);
uint8_t	ASCIItoDEC_s1byte (char* str, sint8_t* result);

uint8_t	ASCIItoDEC_u2byte (char* str, uint16_t* result);
uint8_t	ASCIItoDEC_s2byte (char* str, sint16_t* result);

uint8_t	ASCIItoDEC_u4byte (char* str, uint32_t* result);
uint8_t	ASCIItoDEC_s4byte (char* str, sint32_t* result);

#endif