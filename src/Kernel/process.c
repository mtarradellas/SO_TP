#include <stdlib.h>
#include "include/process.h"
#include "include/memoryManager.h"

#include "include/videoDriver.h"

#define READY 0
#define BLOCKED 1
#define DEFAULT_PROC_MEM 4096  // 4k

uint64_t _initProcess(uint64_t stackBase, int (*entry)(int, char **), int argc, char **argv);

static long int id = 0;

tProcess *newProcess(char *name, int (*entry)(int, char **), int argc,
                     char **argv) {
  
  tProcess *newP = mallocMemory(sizeof(*newP));
  if (newP == NULL) {
  	// throw error
  	putStr("newP NULL");
  	return NULL;
  }
  newP->pid = id++;
  newP->name = name;
  newP->entry = entry;
  newP->argc = argc;
  newP->argv = argv;
  newP->stackBase = mallocMemory(DEFAULT_PROC_MEM);
  if (newP->stackBase == NULL) {
  	// throw error
  	putStr("stack NULL");
  	return NULL;
  }
  newP->stackTop = newP->stackBase - DEFAULT_PROC_MEM;
  newP->rsp = _initProcess(newP->stackBase, newP->entry, newP->argc, newP->argv);
  newP->status = READY;
  return newP;
}