#ifndef memoryManager_h
#define memoryManager_h

#include <stddef.h>
#include <stdint.h>
#include "include/lib.h"

#define MEM_SIZE (1 << 20)
#define MAX_NUM_NODES 5000000

typedef struct listNode {
  size_t freed;
  uint8_t* address;
  size_t size;
  size_t available;
  struct listNode* next;
  struct listNode* prev;
} listNode;

/*
 * Frees a block of memory that was previously allocated.
 */
void free(void* memoryAddress);

/*
 * Reserves a block of contiguous memory of size equal to the argument (there
 * are no controls of incorrect use of memory, it is the responsibility of the
 * user to respect the limits requested). Returns the pointer to said block.
 */
void* malloc(size_t space);

/*
 * Reserves a block of contiguous memory of size equal to the argument (there
 * are no controls of incorrect use of memory, it is the responsibility of the
 * user to respect the limits requested). It also receives a pointer to a block
 * of memory to be copied to the new block. The old block is released. Returns
 * the pointer to the new block.
 */
void* realloc(void* memoryAddress, size_t space);

/*
 * Reserves a block of contiguous memory of size equal to the argument (there
 * are no controls of incorrect use of memory, it is the responsibility of the
 * user to respect the limits requested). Memory is initialized with zeros.
 * Returns the pointer to said block.
 */
void* calloc(size_t space);

void initializeMM();

// not for user
// used for tesing

void printNode(uint8_t* address);
listNode* getBlockNode(uint8_t* address);

#endif
