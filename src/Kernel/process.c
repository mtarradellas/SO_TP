#include <stddef.h>
#include "include/process.h"
#include "include/memoryManager.h"

#include "lib.h"
#include "include/videoDriver.h"

#define DEFAULT_PROC_MEM 4096  // 4k

static long int id;

tProcess *newProcess(char *name, int (*entry)(int, char **), int argc,
                     char **argv, int priority) {
  
  tProcess *newP = (uint64_t)malloc(sizeof(tProcess));
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
  newP->stackBase = (uint64_t)malloc(DEFAULT_PROC_MEM) + DEFAULT_PROC_MEM - 1;
  if (newP->stackBase == NULL) {
  	// throw error
  	putStr("stack NULL");
  	return NULL;
  }
  newP->stackTop = newP->stackBase - DEFAULT_PROC_MEM + 1;
  newP->rsp = newP->stackBase;
  newP->priority = priority;
  newP->status = READY;
  return newP;
}

void initPids() {
  id = 0;
}

void freeProcess(tProcess* process) {
  free(process->stackTop);
  free(process);
}