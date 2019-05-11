#ifndef MEMORYMODULE_H
#define MEMORYMODULE_H

#include <stddef.h>

void* malloc(size_t size);
void* realloc(void* source, size_t size);
void free(void* source);
void printNode(void* source);

#endif