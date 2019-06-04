#include "include/memoryManager.h"
typedef struct list_t {
  struct list_t *next, *prev;
} list_t;

#define TRUE 1
#define FALSE 0
#define BUCKET_AMOUNT 23
#define LEAF_SIZE 128

#define max_blocks_of_level(n) ((1 << (n)))
#define is_leaf_block(n) ((1 << (BUCKET_AMOUNT - 1)) <= (n)) ? TRUE : FALSE
#define move_to_parent(index) (((index)-1) / 2)
#define move_to_left_child(index) ((index)*2 + 1)
#define move_to_right_child(index) ((index)*2 + 2)
#define move_to_sibling(index) ((((index)-1) ^ 1) + 1)
#define array_size(i) (((i) / 8) + (!!((i) % 8)))
#define toggle_bit(array, i) (array[i / 8] ^= 1 << (i % 8))
#define get_bit(array, i) (1 & (array[i / 8] >> (i % 8)))
#define total_size \
  (1 << (BUCKET_AMOUNT - 1)) * LEAF_SIZE  // es como hacer :(1<<30) = 1*(2^30
#define size_of_level(n) (total_size / (1 << (n)))

static int indexInLevelOf(uint8_t *memoryAdr, int level);
static uint8_t* address(int index, int level);
static int bucketRequest(size_t request);
static list_t *popList(int level);
static void pushList(int level, uint8_t *memoryAdr);
static void removeFromList(int level, list_t *node);
static int blockHasBeenSplit(uint8_t *memoryAdr, int level);
static int findLevel(uint8_t *memoryAdr);
static int getNextFreeBlock(int level);
static void splitBlock(int level);


static uint8_t *baseAdress;
extern uint8_t endOfKernel;
static char splitBlocks[array_size(1 << (BUCKET_AMOUNT - 1))] = {0};
static char allocatedBlocks[array_size(1 << (BUCKET_AMOUNT))] = {0};
static list_t *buckets[BUCKET_AMOUNT];


void initializeMM() {
  baseAdress = (uint8_t *)&endOfKernel;
  list_t *node = (list_t *)baseAdress;
  node->next = NULL;
  buckets[0] = node;
}

void *malloc(size_t request) {
  if (request > size_of_level(0)) {
    return NULL;
  }
  if (baseAdress == NULL) {
    initializeMM();
  }
  int level = bucketRequest(request);
  if (!getNextFreeBlock(level)) {
    return NULL;
  }
  list_t *list = popList(level);
  int index = indexInLevelOf((uint8_t *)list, level);
  toggle_bit(allocatedBlocks, index);
  return (void *)list;
}

void free(void *memoryAdr) {
  if ((memoryAdr == NULL) || ((void*)memoryAdr < (void*)baseAdress) ||
      ((void*)memoryAdr > ((void*)baseAdress + total_size))) {
    return;
  }
  int level = findLevel(memoryAdr);
  int index = indexInLevelOf(memoryAdr, level);
  toggle_bit(allocatedBlocks, index);

  while (index > 0) {
    if (get_bit(allocatedBlocks, move_to_sibling(index)) ||
        get_bit(splitBlocks, move_to_sibling(index))) {
      break;
    }
    removeFromList(level, (list_t *)address(move_to_sibling(index), level));
    index = move_to_parent(index);
    toggle_bit(splitBlocks, index);
    level--;
  }
  pushList(level, address(index, level));
}

void *realloc(void *memorySrc, size_t request) {
  int level = findLevel(memorySrc);
  char size = size_of_level(level);
  if (size >= request) {
    return memorySrc;
  }
  char *memoryAdr = malloc(request);
  for (int i = 0; i < size; i++) {
    *(memoryAdr + i) = *((char *)memorySrc + i);
  }
  free(memorySrc);
  return memoryAdr;
}

void *calloc(size_t request) {
  char *memoryAdr = malloc(request);
  for (int i = 0; i < request; i++) {
    *(memoryAdr + i) = 0;
  }
  return memoryAdr;
}

static int indexInLevelOf(uint8_t *memoryAdr, int level) {
  return ((memoryAdr - baseAdress) >> (29 - level)) + (1 << level) - 1;
}

static uint8_t* address(int index, int level) {
  return baseAdress + ((index - (1 << level) + 1) << (29 - level));
}

static int bucketRequest(size_t request) {
  int bucket = BUCKET_AMOUNT - 1;
  int size = LEAF_SIZE;
  while (size < request) {
    bucket--;
    size *= 2;
  }
  return bucket;
}

static list_t *popList(int level) {
  list_t *aux = buckets[level];
  buckets[level] = aux->next;
  buckets[level]->prev = NULL;
  return aux;
}

static void pushList(int level, uint8_t *memoryAdr) {
  list_t *node = (list_t *)((void*)memoryAdr);
  node->next = buckets[level];
  node->prev = NULL;
  buckets[level]->prev = node;
  buckets[level] = node;
}

static void removeFromList(int level, list_t *node) {
  if (node == buckets[level]) {
    popList(level);
    return;
  }
  list_t *next = node->next;
  list_t *prev = node->prev;
  if (prev != NULL) {
    prev->next = next;
  }
  if (next != NULL) {
    next->prev = prev;
  }
}

static int blockHasBeenSplit(uint8_t *memoryAdr, int level) {
  int index = indexInLevelOf(memoryAdr, level);
  return get_bit(splitBlocks, index);
}

static int findLevel(uint8_t *memoryAdr) {
  int n = BUCKET_AMOUNT - 1;
  while (n > 0) {
    if (blockHasBeenSplit(memoryAdr, (n - 1))) {
      return n;
    }
    n = n - 1;
  }
  return 0;
}

static int getNextFreeBlock(int level) {
  int i = level;
  while ((buckets[i] == NULL) && (i >= 0)) {
    i--;
  }
  if (i < 0) {
    return FALSE;
  }
  while (i < level) {
    splitBlock(i++);
  }
  return TRUE;
}

static void splitBlock(int level) {
  list_t *aux = popList(level);
  int index = indexInLevelOf((uint8_t*)aux, level);
  toggle_bit(splitBlocks, index);
  pushList(level + 1, address(move_to_right_child(index), level + 1));
  pushList(level + 1, address(move_to_left_child(index), level + 1));
}

