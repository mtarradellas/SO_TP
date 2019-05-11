/********************************************************************************************************
 *    Managable memoy is divided into two sections: one reserved for the Nodes (internal use)     *
 *                and the next is for external use                    *
 *    Nodes: a list begining in "memory" where each node represents a partition of the memory;    *
 *      each contains information for said partition (size, address, availability)          *
 ********************************************************************************************************/
#include "include/memoryManager.h"
#include "include/videoDriver.h"

static uint8_t * baseAddress;
static const uint8_t * startAddress =(uint8_t *) 0x1000000;
static listNode * memory;
static size_t numNodes;

listNode * getNextNodeAddress();
void joinNodes(listNode * node);
void resizing(listNode * bestFit, size_t space);
listNode * getNextAvailableBlock(listNode * node);
listNode * getBlockNode(uint8_t * address);
listNode * getBestFitNode(size_t space);


void * malloc(size_t space) {

  listNode * bestFit = getBestFitNode(space);

    if (bestFit == NULL){
      return NULL;
    }

    bestFit->available = 0;

    if(bestFit->size > space){
      resizing(bestFit, space);
    }
    return bestFit->address;
}

void * realloc(void * memoryAddress, size_t space) {

  listNode * bestFitNode = getBestFitNode(space);
  listNode * oldNode = getBlockNode(memoryAddress);

  if (bestFitNode == NULL){
    if (oldNode->size < space){
      return NULL;
    }
    if (oldNode->size > space){

      resizing(oldNode, space);
    }
    return oldNode->address;
  }

    for (int i = 0; i < oldNode->size; i++) {
         *((bestFitNode->address)+i) = *((char *)memoryAddress+i);
    }
    free(memoryAddress);

    bestFitNode->available = 0;

    if(bestFitNode->size > space){
      resizing(bestFitNode, space);
    }
  joinNodes(oldNode);
    return bestFitNode->address;
}

void * calloc(size_t space) {

    char * retAddress = malloc(space);
    for (int i = 0; i < space; i++) {
        *(retAddress+i) = 0;
    }
    return retAddress;
}

void free(void * memoryAddress) {

  if ((memoryAddress == NULL) || ((uint8_t *)memoryAddress < baseAddress) || ((uint8_t *)memoryAddress > (baseAddress + MEM_SIZE))) {
    return;
  }

  listNode * oldNode = getBlockNode((uint8_t *)memoryAddress);
  oldNode->available = 1;
  
}


// ****************     a      ********************
// ****************     u      ********************
// ****************     x      ********************



void initializeMM() {
    memory = (listNode *)startAddress;
    numNodes = 1;
    baseAddress = (uint8_t *)memory + (sizeof(listNode) * MAX_NUM_NODES);

    memory->address = baseAddress;
    memory->freed = 0;
    memory->size = MEM_SIZE;
    memory->available = 1;
    memory->next = NULL;
    memory->prev = NULL;
}

listNode * getNextNodeAddress(){

  listNode * nextNodeAddress = memory;
  for(int i = 1; i <= numNodes; i++){
    if(nextNodeAddress->freed){
      return nextNodeAddress;
    }
    nextNodeAddress = nextNodeAddress + sizeof(listNode);
  }
  numNodes++;
  return nextNodeAddress;

}


// returns first free partition
listNode * getNextAvailableBlock(listNode * node) {
  if (node == NULL){
    return NULL;
  }
  while(! node->available){
       if(node->next == NULL){
        return NULL;
       }
       node = node->next;
    }

    return node;
}


// given an address returns the node corresponding to that address if it exists
listNode * getBlockNode(uint8_t * address) {
  
  if ((address == NULL) || (address < baseAddress) || (address > (baseAddress + MEM_SIZE))){
    
    if(address == NULL){
      putStr("This node does not exist or has been freed\n");
    }
    putStr("Invalid address\n");
    return NULL;        
  }

  listNode * aux = memory;

  while (aux != NULL){
        
    if (aux->address == address){
      return aux;
    }
    aux = aux->next;
  }


  return NULL;
}


// returns best fit (a partition which is the smallest sufficient partition among the free available partitions)
// or null if best fit does not exist
listNode * getBestFitNode(size_t space) {

  listNode * aux;
  listNode * bestFit=NULL;

    if (baseAddress == NULL) {
        initializeMM();
    }
  if (memory->available && memory->size >= space){
    bestFit = memory;
  }

  aux = getNextAvailableBlock(memory->next);

    while (aux != NULL) {

        if (aux->size >= space && (bestFit == NULL || aux->size < bestFit->size)) {
        bestFit = aux;

      }
      aux = getNextAvailableBlock(aux->next);

    }
   return bestFit;
}

//bestfit is the to use node with extra space 
// it is divided into two nodes, the old one and he new one with the extra space
void resizing(listNode * bestFit, size_t space) {

    listNode * node = getNextNodeAddress();

      node->next = bestFit->next;
      node->available = 1;
      node->freed = 0;
      node->prev = bestFit;
      node->address = (bestFit->address)+space;
      node->size = (bestFit->size)-space;
      bestFit->size = space;
      bestFit->next = node;
      if(node->next != NULL){
        node->next->prev = node;
      }

      joinNodes(node);
}

// if two continuous partitions are free, they are joined into a unified bigger one
void joinNodes(listNode * node) {


  if (node->next != NULL && node->next->available){
    node->next->freed = 1;
    node->size += node->next->size;
    if(node->next->next != NULL){
        node->next->next->prev = node;
      }
      node->next->address = NULL;
    node->next = node->next->next;
  }

  if (node->prev != NULL && node->prev->available){
    node->prev->size += node->size;
    node->prev->next = node->next;
    if(node->next != NULL){
        node->next->prev = node->prev;
      }
      node->address = NULL;
  }

}

// used for testing solo usar si el content es un string
void printNode(uint8_t * address) {

  listNode * node = getBlockNode(address);
  if(node == NULL){
    putStr("Invalid node: this node does not exist or has been freed \n");
    putStr("----");
    newLine();
    return;
  }
  putStr("\n----\n");
  putStr("content: ");
  putStr((char *)node->address);
  newLine();

  putStr("address ");
  char buff[10];
  putStr(decToStr((size_t)node->address,buff));
  newLine();

  putStr("size: ");
  char buffer[10];
  putStr(decToStr((size_t)node->size,buffer));
  newLine();

  putStr("available: ");
  if(node->available == 0){
    putStr("NO");
  }
  else{
    putStr("YES");
  }
  newLine();
  
  putStr("----");
  newLine();



}