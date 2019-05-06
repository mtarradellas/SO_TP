#include <stdint.h>
#include <stddef.h>
#include "include/memoryModule.h"
#include "include/SYSCall.h"

void* malloc(size_t size) {
  void* dest;
  systemCall((uint64_t)MALLOC, (uint64_t)&dest, (uint64_t)size, 0, 0, 0);
  return dest;
}

void* realloc(void* source, size_t size) {
  void* dest;
  systemCall((uint64_t)REALLOC, (uint64_t)source, (uint64_t)size, (uint64_t)&dest, 0, 0);
  return dest;
}

void free(void* source) {
  systemCall((uint64_t)FREE, (uint64_t)source, 0, 0, 0, 0);
}