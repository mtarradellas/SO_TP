#ifndef buddySystemMemManager_h
#define buddySystemMemManager_h

#include <stddef.h>
#include <stdint.h>

#include "lib.h"

#define MEM_SIZE (1 << 20)
#define MIN_BLOCK_SIZE 128
#define LEVELS (20 - 7 +  1)  // 1???? //each power from 7 to 20 will be a level (each with a different
       // size of block). starting from seven because the smallest block will be
       // of size 2^7 and the max will be the whole memory 2^20
#define MAX_NUM_NODES (1 << 13)  // max_blocks_in_level in last level
#define BIGGEST_SIZE_LEVEL 0
#define SMALLEST_SIZE_LEVEL 13

//#define log2(i) (1>>i)
#define level_size(i) ((MEM_SIZE) / ((1) << (i)))  // size of blocks in a given level
#define get_level_index(i) ((20) - (i))
#define max_blocks_in_level(n) ((1) << (n))

typedef struct listNode {  // WILL I NEED ALL OF THIS SHIT AGAIN??
  size_t freed;
  uint8_t* address;
  size_t level;
  size_t available;
  struct listNode* next;
  struct listNode* rightBuddy;
  struct listNode* parent;
  struct listNode* prev;
} listNode;

/*
 * Frees a block of memory that was previously allocated.
 */
void free(void* memoryAddress);

/*
 * Reserves a block of contiguous memory of size equal to (or the size of first
 * power of 2 bigger than) the argument (there are no controls of incorrect use
 * of memory, it is the responsibility of the user to respect the limits
 * requested). Returns the pointer to said block.
 */
void* malloc(size_t space);

/*
 * Reserves a block of contiguous memory of size equal to (or the size of first
 * power of 2 bigger than) the argument (there are no controls of incorrect use
 * of memory, it is the responsibility of the user to respect the limits
 * requested). It also receives a pointer to a block of memory to be copied to
 * the new block. The old block is released. Returns the pointer to the new
 * block.
 */
void* realloc(void* memoryAddress, size_t space);

//for scheduler only
// Sets up the memory manager
void initializeMM();

// not for user
// used for tesing

// used for tesing

//Given an address, searches the corresponding node and prints it
void printNode(uint8_t* address);
//Given an address, returns the corresponding node
listNode* getBlockNode(uint8_t* address);
// Prints the information of a given node
void printNode2(listNode *node);

#endif
