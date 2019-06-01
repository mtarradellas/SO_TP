#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <stdint.h>
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

void printf(char* fmt, ...);
void _cli();
void _go_to(void* sp);

typedef int (*EntryPoint)();

void* getEntryPoint() {
  return sampleCodeModuleAddress;
}

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
  //schedTestDinamic();

  printf("welcome goma\n");

 char * lenia = malloc(128);
  // printNode(lenia);
 // printNode(lenia);
  return 1;
}
