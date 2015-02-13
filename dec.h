#ifndef _DEC_H_
#define _DEC_H_

uint32_t CheckInt (char* str, char** OutStr);

uint8_t	ASCIItoDEC_u1byte (char* str, char* result);
uint8_t	ASCIItoDEC_s1byte (char* str, char* result);

uint8_t	ASCIItoDEC_u2byte (char* str, char* result);
uint8_t	ASCIItoDEC_s2byte (char* str, char* result);

uint8_t	ASCIItoDEC_u4byte (char* str, char* result);
uint8_t	ASCIItoDEC_s4byte (char* str, char* result);

uint32_t power10 (uint8_t power);

#endif
