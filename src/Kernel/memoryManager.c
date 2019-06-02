#include "include/memoryManager.h"
#include "include/lib.h"
#include "include/videoDriver.h"

static uint8_t *baseAddress;  // where the memory starts (after node
                              // information)
static const uint8_t *startAddress =
    (uint8_t *)0x1000000;  // where the node information starts

static listNode *levelArr[LEVELS];  // Each level corresponds to a certain
                                    // allocation size and stores
// a free list. Level 0 corresponds to BIGGEST_SIZE_LEVEL and the biggest
// possible allocation

// for navigation and use of the node structures
static size_t numNodes;
static listNode *getNextNodeAddress();

// aux
static listNode *getBestFitNode(size_t space);
static int levelHasAvailable(int level);
listNode *getBlockNode(uint8_t *address);
static int optimalLevel(size_t space);
static void splitBlock(int level);
static listNode *getNextAvailableBlock(listNode *node);
static void mergeNodes(listNode *node);
static void deleteNode(listNode *node);

void printNode2(listNode *node);

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
  // printNode2(bestFit);//
  return bestFit->address;
}

void *realloc(void *memoryAddress, size_t space) { return baseAddress; }

// void * calloc(size_t space) {

//}

void free(void *memoryAddress) {
  if ((memoryAddress == NULL) || ((uint8_t *)memoryAddress < baseAddress) ||
      ((uint8_t *)memoryAddress > (baseAddress + MEM_SIZE))) {
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

// ****************     a      ********************
// ****************     u      ********************
// ****************     x      ********************

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
  // printNode2(node);
}

static listNode *getBestFitNode(size_t space) {
  int opLevel = optimalLevel(space);

  int level = opLevel;

  while ((!levelHasAvailable(level)) && (level > BIGGEST_SIZE_LEVEL)) {
    level--;
  }

  if ((level = BIGGEST_SIZE_LEVEL) && (!levelArr[level]->available)) {
    return NULL;
  }
  while (opLevel > level) {
    splitBlock(level);
    level++;
  }
  // printNode2(getNextAvailableBlock(levelArr[opLevel]));
  return getNextAvailableBlock(levelArr[opLevel]);
}

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

// given an address returns the node corresponding to that address if it exists
listNode *getBlockNode(uint8_t *address) {
  if ((address == NULL) || (address < baseAddress) ||
      (address > (baseAddress + MEM_SIZE))) {
    // nulll para freed?
    if ((address == NULL)) {
      printf("addres es null\n");
    } else if ((address < baseAddress)) {
      printf("address es menor a baseadress\n");
    } else if (address > (baseAddress + MEM_SIZE)) {
      printf("address es mayor a baseadress + memzise\n");
    }
    return NULL;
  }

  listNode *aux = (listNode *)startAddress;
  listNode *ret = NULL;

  while (aux != NULL) {
    if (aux->address == address) {
      ret = aux;
    }
    aux = aux->next;
  }
  // printNode2(ret);
  return ret;
}

static int optimalLevel(size_t space) {
  int i = SMALLEST_SIZE_LEVEL;
  for (; i >= 0; i--) {
    if (space <= level_size(i)) {
      return i;
    }
  }
  return -1;
}

static void splitBlock(int level) {
  listNode *node = getNextAvailableBlock(levelArr[level]);
  ////printNode2(node);
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
  nodeRight->address = node->address + level_size(level + 1);
  nodeRight->level = level + 1;
  nodeRight->rightBuddy = NULL;
  nodeLeft->parent = node;
  nodeRight->prev = nodeLeft;
  nodeRight->next = levelArr[level + 1];

  levelArr[level + 1] = nodeLeft;
}

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

static void mergeNodes(listNode *node) {
  if (node->rightBuddy == NULL) {
    if (node->prev != NULL) {
      mergeNodes(node->prev);
    }
    return;
  }
  if (!node->rightBuddy->available) {
    return;
  }  // if node got to this point, it is a left node
  listNode *rightNode = node->rightBuddy;
  listNode *parent = node->parent;

  deleteNode(node);
  deleteNode(rightNode);
  parent->available = 1;
  mergeNodes(parent);
}

static void deleteNode(listNode *node) {
  node->prev->next = node->next;
  node->freed = 1;
}

// static listNode * getParent(listNode * leftChild) {

//   int parentLevel = leftChild->level - 1;
//   listNode * parent = levelArr[parentLevel];

//   while(parent != NULL){
//     if (parent->address == leftChild-> address){
//       return parent;
//     }
//     parent = parent->next;
//   }
//   return NULL;
// }

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

void printNode2(listNode *node) {
  if (node == NULL) {
    printf("Invalid node: this node does not exist or has been freed\n");
    return;
  }
  printf("content: %d\n", node->address);
  printf("\n-----\n");
  printf("address: %d\n", node->address);
  printf("level: %d\n", node->level);
  printf("available: %s\n", (node->available == 0) ? "NO" : "YES");
  printf("\n----\n");
}

void printNode(uint8_t *address) {
  printf("address en printNode: %d\n");
  listNode *node = getBlockNode(address);
  printNode2(node);
}
