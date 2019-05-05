#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <string.h>
#include "IDTLoader.h"
#include "scheduler.h"
#include "videoDriver.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void* const sampleCodeModuleAddress = (void*)0x400000;
static void* const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();

void clearBSS(void* bssAddress, uint64_t bssSize) {
  memset(bssAddress, 0, bssSize);
}

void* getStackBase() {
  return (void*)((uint64_t)&endOfKernel +
                 PageSize * 8          // The size of the stack itself, 32KiB
                 - sizeof(uint64_t));  // Begin at the top of the stack
}

void* initializeKernelBinary() {
  void* moduleAddresses[] = {sampleCodeModuleAddress, sampleDataModuleAddress};

  loadModules(&endOfKernelBinary, moduleAddresses);

  clearBSS(&bss, &endOfKernel - &bss);

  return getStackBase();
}

int main() {
  _cli();
  loadIDT();  
  //schedTest((uint64_t)sampleCodeModuleAddress);

  //return 1;
 
  char * ptr = (char *)calloc(6);


  memcpy(ptr, "Lenia", sizeof("Lenia"));

  printNode(getBlockNode(ptr));
  putStr("00000000000000000000000000000\n");


  char * ptr2 = (char *) realloc(ptr, 25);

  memcpy(ptr2+5, " Y franco", sizeof(" Y franco"));




  printNode(getBlockNode(ptr));
  printNode(getBlockNode(ptr2));

  putStr("00000000000000000000000000000\n");

  char * ptr3 = (char *) realloc(ptr2, 29);

  memcpy(ptr3+14, " Y alejo", sizeof(" Y alejo"));

  printNode(getBlockNode(ptr));
  printNode(getBlockNode(ptr2));
  printNode(getBlockNode(ptr3));
  putStr("00000000000000000000000000000\n");

  char * ptr4 = (char *) malloc(43);
  memcpy(ptr4, "babyy", sizeof("babyy"));
  printNode(getBlockNode(ptr));
  printNode(getBlockNode(ptr2));
  printNode(getBlockNode(ptr3));
  printNode(getBlockNode(ptr4));

  return 1; 
}
