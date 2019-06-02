#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void *memset(void *destination, int32_t character, uint64_t length);
void *memcpy(void *destination, const void *source, uint64_t length);

// Transformes a decimal to string. used for displaying the registers
char *decToStr(int num, char *buffer);
int strcmp(char* a, char* b);
int strlen(char* str);
unsigned long int rand();
int read(int fd, char* buffer, int size);
int write(int fd, char *buffer, int size);
char *cpuVendor(char *result);

/////TEST
void printf(char* fmt, ...);

#endif
