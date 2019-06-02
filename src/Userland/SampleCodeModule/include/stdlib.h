#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>
#include "./shell.h"

// Prints string with formats
void printf(char* fmt, ...);

// Prints char
void putChar(char c);

// Prints decimal
void putDec(int i);

// Reads a char from buffer
char getChar();

// Trensforms a decimal to string
char* decToStr(int num, char* buffer);

// Scans from buffer and prints the contents scanned
void scanAndPrint(char* buffer);

// Scans from buffer
void scan(char* buffer);

// Compares two strings
int strCmp(char* a, char* b);

// Returns string length
int strLen(char* str);

// Clears buffer
void clearBuffer(char* buffer);

// Returns absolut value of given int
int abs(int n);

// Prints string
void putStr(char* word);

int atoi(char* str);

unsigned long int rand();


void *memcpy(void *destination, const void *source, uint64_t length);


void splitString(char* buffer, char output[MAX_ARGUMENTS][MAXLEN], int argc);

#endif
