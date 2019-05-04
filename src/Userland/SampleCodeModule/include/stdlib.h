#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>

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

// Compares two strings
int strCmp(char* a, char* b);

// Clears buffer
void clearBuffer(char* buffer);

// Returns absolut value of given int
int abs(int n);

// Prints string
void putStr(char* word);

#endif
