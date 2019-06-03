#include <moduleLoader.h>
#include <naiveConsole.h>
#include <stdint.h>
#include "include/IDTLoader.h"
#include "include/lib.h"
#include "include/memoryManager.h"
#include "include/scheduler.h"
#include "include/videoDriver.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void* const sampleCodeModuleAddress = (void*)0x400000;
static void* const sampleDataModuleAddress = (void*)0x500000;

void _cli();
void _go_to(void* sp);
static void testMem();

typedef int (*EntryPoint)();

void* getEntryPoint() { return sampleCodeModuleAddress; }

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
  _go_to(getStackBase());
  loadIDT();
   //start((EntryPoint)sampleCodeModuleAddress);
  testMem();
  return 1;
}


static void testMem() {

  char* lenia1 = malloc(1025);
  char* lenia2 = malloc(1024);
  //char* lenia3 = malloc(228);
  char* lenia4 = malloc(55528);

  memcpy(lenia1, "el", sizeof("el")+1);
  memcpy(lenia2, "malloc", sizeof("malloc")+1);
  //memcpy(lenia3, "anda", sizeof("anda")+1);
  memcpy(lenia4, "bitch", sizeof("bitch")+1);

  printf("%s\n", "lenia1" );
  printNode(lenia1);
  printf("%s\n", "lenia2" );
  printNode(lenia2);
  //printf("%s\n", "lenia3" );
  //printNode(lenia3);
  printf("%s\n", "lenia4" );
  printNode(lenia4);

  printf("freeing lenia2:\n");
  free(lenia2);
  printf("%s\n", "lenia2" );
  printNode(lenia2);


  printf("freeing lenia1:\n");
  free(lenia1);
  printf("%s\n", "lenia1" );
  printNode(lenia1);

}


