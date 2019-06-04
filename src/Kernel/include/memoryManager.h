#ifndef BuddyAllocationSystem_h
#define BuddyAllocationSystem_h

#include <stddef.h>

#include "./lib.h"
#include "./videoDriver.h"

void free(void* memoryAdr);

void* malloc(size_t request);

void* realloc(void* memorySrc, size_t request);

void* calloc(size_t request);


void initializeMM();

#endif
