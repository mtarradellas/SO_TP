
#include "include/buddySystemMemManager.h"
#include "include/videoDriver.h"

static uint8_t * baseAddress; //where the memory starts
static const uint8_t * startAddress =(uint8_t *) 0x1000000; //where the nodes information starts

static list_t * levelArr[LEVELS]; // Each level corresponds to a certain allocation size and stores a free list. Level 0 corresponds to BIGGEST_SIZE_LEVEL and the biggest possible allocation
static size_t numNodes;

static listNode * getBlockNode(uint8_t * address);
static listNode * getNextNodeAddress();
static int optimalLevel(size_t space)

void * malloc(size_t space) {

  if (space > level_size(BIGGEST_SIZE_LEVEL)) {
      return NULL;
  }
  if (baseAdress == NULL) {
      initializeMM();
  }

  int level = optimalLevel(space);
  if (!getNextFreeBlock(level)) {
      return NULL;
  }
    list_t * list = popList(level);
    int index = indexInLevelOf((uint8_t *)list, level);
    toggle_bit(allocatedBlocks, index);
    return (void *)list;
}

void * realloc(void * memoryAddress, size_t space) {


}

void * calloc(size_t space) {

}

void free(void * memoryAddress) {
  
}


// ****************     a      ********************
// ****************     u      ********************
// ****************     x      ********************



void initializeMM() {
  baseAddress = (uint8_t *)startAddress + (sizeof(listNode) * MAX_NUM_NODES) + sizeof(levelArr);
  numNodes = 1;
  listNode * node = (listNode *)startAddress;

  memory->address = baseAddress;
  memory->freed = 0;
  memory->size = MEM_SIZE;
  memory->available = 1;
  memory->next = NULL;
  memory->prev = NULL;

  levelArr[BIGGEST_SIZE_LEVEL];
}

static listNode * getNextNodeAddress(){

  listNode * nextNodeAddress = (listNode *)startAddress;
  for(int i = 1; i <= numNodes; i++){
    if(nextNodeAddress->freed){
      return nextNodeAddress;
    }
    nextNodeAddress = nextNodeAddress + sizeof(listNode);
  }
  numNodes++;
  return nextNodeAddress;

}

// given an address returns the node corresponding to that address if it exists
static listNode * getBlockNode(uint8_t * address) {
  
  if ((address == NULL) || (address < baseAddress) || (address > (baseAddress + MEM_SIZE))){
    return NULL;        
  }

  listNode * aux = (listNode *) startAddress;

  while (aux != NULL){
        
    if (aux->address == address){
      return aux;
    }
    aux = aux->next;
  }
  return NULL;
}

static int optimalLevel(size_t space){
  for(int i = SMALLEST_SIZE_LEVEL; i>=0; i--){
    if(space <= level_size(i)){
      return i;
    }
  }
}

static ?? getNextAvailableBlock(int level){ // devuelve el block???
  int optimal level = level;

  while((levelArr[level] == NULL) && (level > BIGGEST_SIZE_LEVEL)) { level--;}

  if(level = BIGGEST_SIZE_LEVEL && (levelArr[level] == NULL)){
    return NULL;
  }
  if(optimalLevel > level){
    splitBlock(level);
  }
}


void splitBlock(int level) {    // devuelve el block???
    listNode * node = levelArr[level];
    levelArr[level] = levelArr[level]->next;
    levelArr[level]->prev = NULL;


    int index = indexInLevelOf(node, level);
    return((memoryAdr - baseAdress) >> (29 - level)) + (1 << level) - 1;
    toggle_bit(splitBlocks,index);
    pushList(level+1, adress(move_to_right_child(index), level+1));
    pushList(level+1, adress(move_to_left_child(index), level+1));
}



#define is_leaf_block(n) ((1 << (BUCKET_AMOUNT - 1)) <= (n)) ? TRUE : FALSE
#define move_to_parent(index) (((index) - 1) / 2)
#define move_to_left_child(index) ((index) * 2 + 1)
#define move_to_right_child(index) ((index) * 2 + 2)
#define move_to_sibling(index) ((((index) - 1) ^ 1) + 1)
#define array_size(i) (((i) / 8) + (!! ((i) % 8)))
#define toggle_bit(array, i) (array[i / 8] ^= 1 << (i % 8))
#define get_bit(array, i) (1 & (array[i / 8] >> (i % 8)))