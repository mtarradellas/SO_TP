#include "include/memoryManager.h"
#include "include/lib.h"
#include "include/videoDriver.h"

// Where the memory starts (after node information).
static uint8_t *baseAddress; 
// Where the node information starts.
static const uint8_t *startAddress = (uint8_t *)0x1000000;  

/*  
 *  Each level corresponds to a certain allocation size and stores a free list. 
 *  Level 0 corresponds to BIGGEST_SIZE_LEVEL and the biggest possible allocation (2^20).
 *  Level 13 corresponds SMALLEST_SIZE_LEVEL and the smallest possible allocation (2^7).
 */
static listNode *levelArr[LEVELS];

// For navigation and use of the node structures
static size_t numNodes;
static listNode *getNextNodeAddress();

// Auxiliary functions
static listNode *getBestFitNode(size_t space);
static int levelHasAvailable(int level);
static int optimalLevel(size_t space);
static void splitBlock(int level);
static listNode *getNextAvailableBlock(listNode *node);
static void mergeNodes(listNode *node);
static void deleteNode(listNode *node);
static getBlockNodeLevel(listNode * node, uint8_t *address);


void *malloc(size_t space) {

  if (space > MEM_SIZE) {
    return NULL;
  }
  if (baseAddress == NULL) {
    initializeMM();
  }
  listNode *bestFit = getBestFitNode(space);

  if (bestFit == NULL) {
    return NULL;
  }
  bestFit->available = 0;
  return bestFit->address;
}

void *realloc(void *memoryAddress, size_t space) { 
  
  if (memoryAddress == NULL) {
    return malloc(space);
  }
  if (baseAddress == NULL) {
    initializeMM();
  }
  if(((uint8_t *)memoryAddress < baseAddress) || ((uint8_t *)memoryAddress > (baseAddress + MEM_SIZE))){
    return NULL;
  }
  listNode * bestFitNode = getBestFitNode(space);
  if(bestFitNode == NULL){
    return NULL;
  }
  for (int i = 0; i < space; i++) {
    *((bestFitNode->address)+i) = *((char *)memoryAddress+i);
  }
  free(memoryAddress);
  return bestFitNode->address; 
}

void free(void *memoryAddress) {
  if ((memoryAddress == NULL) || ((uint8_t *)memoryAddress < baseAddress) ||
      ((uint8_t *)memoryAddress > (baseAddress + MEM_SIZE))) {
    printf("hello 1\n");
    return;
  }
  listNode *node = getBlockNode(memoryAddress);
  if (node == NULL) {
    return;
  }
  node->freed = 1;
  node->available = 1;
  mergeNodes(node);
}



// ****************     A      ********************
// ****************     U      ********************
// ****************     X      ********************


/*
 *  Sets up the memory manager
 */
void initializeMM() {
  for (int i = BIGGEST_SIZE_LEVEL; i <= SMALLEST_SIZE_LEVEL; i++) {
    levelArr[i] = NULL;
  }
  baseAddress = (uint8_t *)startAddress + (sizeof(listNode) * MAX_NUM_NODES) +
                sizeof(levelArr);

  numNodes = 1;
  listNode *node = (listNode *)startAddress;
  node->address = baseAddress;
  node->freed = 0;
  node->level = BIGGEST_SIZE_LEVEL;
  node->available = 1;
  node->next = NULL;
  node->rightBuddy = NULL;
  node->parent = NULL;
  node->prev = NULL;

  levelArr[BIGGEST_SIZE_LEVEL] = node;
}
/*
 *  Returns a block node of the smallest possible size that satisfies that 
 *  the space requested is bigger or equal than the space provided.
 */ 
static listNode *getBestFitNode(size_t space) {

  int opLevel = optimalLevel(space);
  int level = opLevel;

  while ((!levelHasAvailable(level)) && (level > BIGGEST_SIZE_LEVEL)) {
    level--;
  }

  if ((level == BIGGEST_SIZE_LEVEL) && (!levelArr[level]->available)) {
    return NULL;
  }
  while (opLevel > level) {
    splitBlock(level);
    level++;
  }
  listNode * aux = getNextAvailableBlock(levelArr[opLevel]);
  
  return aux;
}


/*
 * Returns 1 if the given level has available nodes, else returns 0.
 */
static int levelHasAvailable(int level) {
  if (levelArr[level] == NULL) {
    return 0;
  }
  listNode *node = levelArr[level];
  while (node != NULL) {
    if (node->available) {
      return 1;
    }
    node = node->next;
  }
  return 0;
}

/*
 *  Given an address returns the node corresponding to that address if it exists. 
 *  Searches for the node in all levels, starting from the smallest size level.
 */
listNode *getBlockNode(uint8_t *address) {
  if ((address == NULL) || (address < baseAddress) ||
      (address > (baseAddress + MEM_SIZE))) {
    return NULL;
  }
  listNode *aux;
  for(int i = SMALLEST_SIZE_LEVEL ; i >= BIGGEST_SIZE_LEVEL ; i--){
    aux = getBlockNodeLevel(levelArr[i], address);
    if(aux != NULL){
      return aux;
    }
  }
  return aux;
}

/*
 * Given an address returns the node corresponding to that address if it exists.
 * Searches only in one level (the one corresponding to the given node).
 */
static getBlockNodeLevel(listNode * node, uint8_t *address){
  while(node != NULL){
    if(node->address == address){
      return node;
    }
    node = node->next;
  }
  return node;
}

/*
 * Returns as an int the highest level (the higher the level the smallest its size) in which the level's size 
 * satisfies that the space requested is bigger or equal than the space of the level.
 */
static int optimalLevel(size_t space) {
  int i = SMALLEST_SIZE_LEVEL;
  for (; i >= 0; i--) {
    if (space <= level_size(i)) {
      return i;
    }
  }
  return -1;
}

/*
 *  Given a node, creates its two corresponding children (in the next level) and marks the node as unavailable.
 */
static void splitBlock(int level) {
  listNode *node = getNextAvailableBlock(levelArr[level]);
  if(node == NULL){
    return NULL;
  }
  node->available = 0;

  listNode *nodeLeft = getNextNodeAddress();
  listNode *nodeRight = getNextNodeAddress();

  nodeLeft->available = 1;
  nodeLeft->freed = 0;
  nodeLeft->address = node->address;
  nodeLeft->level = (level + 1);
  nodeLeft->rightBuddy = nodeRight;
  nodeLeft->parent = node;
  nodeLeft->prev = NULL;
  nodeLeft->next = nodeRight;

  nodeRight->available = 1;
  nodeRight->freed = 0;
  nodeRight->address = node->address + (level_size(level + 1));
  nodeRight->level = level + 1;
  nodeRight->rightBuddy = NULL;
  nodeLeft->parent = node;
  nodeRight->prev = nodeLeft;
  nodeRight->next = levelArr[level + 1];

  levelArr[level + 1] = nodeLeft;
}

/*
 * Given a list, returns the first available node or NULL.
 */
static listNode *getNextAvailableBlock(listNode *node) {
  if (node == NULL) {
    return NULL;
  }
  while (!node->available) {
    if (node->next == NULL) {
      return NULL;
    }
    node = node->next;
  }
  return node;
}

/*
 * If a buddy pair is free, eliminates both nodes and marks thir parent as available.
 */
static void mergeNodes(listNode *node) {
  if(!node->available){
    return;
  }
  if (node->rightBuddy == NULL) {
    if (node->prev != NULL) {
      mergeNodes(node->prev);
    }
    return;
  }
  if (!node->rightBuddy->available) {
    return;
  }
  listNode *rightNode = node->rightBuddy;
  listNode *parent = node->parent;
  deleteNode(rightNode);
  deleteNode(node);
  
  parent->available = 1;
  mergeNodes(parent);
}

/*
 * Deletes a node from its list.
 */
static void deleteNode(listNode *node) {
  if(node->prev == NULL){
    levelArr[node->level]= node->next; 
  }
  else{
    node->prev->next = node->next;
  }
  node->freed = 1;
}

/*
 * Used to assign memory spaces to the nodes' structure.
 */
static listNode *getNextNodeAddress() {
  listNode *nextNodeAddress = (listNode *)startAddress;
  for (int i = 1; i <= numNodes; i++) {
    if (nextNodeAddress->freed) {
      nextNodeAddress->freed = 0;
      return nextNodeAddress;
    }
    nextNodeAddress = nextNodeAddress + sizeof(listNode);
  }
  numNodes++;
  return nextNodeAddress;
}

/*
 * Prints the information of a given node.
 */
void printNode2(listNode *node) {
  if (node == NULL) {
    printf("Invalid node: this node does not exist or has been freed\n");
    return;
  }
  printf("\n-----\n");
  printf("content: %s\n", node->address);
  printf("address: %d\n", node->address);
  printf("level: %d\n", node->level);
  printf("available: %s\n", (node->available == 0) ? "NO" : "YES");
  printf("----\n");
}

/*
 * Given an address, searches the corresponding node and prints it.
 */
void printNode(uint8_t *address) {
  listNode *node = getBlockNode(address);
  printNode2(node);
}
