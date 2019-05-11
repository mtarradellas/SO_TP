#include <stddef.h>
#include "include/process.h"
#include "include/memoryManager.h"

#include "lib.h"
#include "include/videoDriver.h"

#define DEFAULT_PROC_MEM 4096  // 4k

static long int id;

tProcess *newProcess(char *name, int (*entry)(int, char **), int argc,
                     char **argv, int priority) {
  
  tProcess *newP = malloc(sizeof(tProcess));
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
  if ((void*) newP->stackBase == NULL) {
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
  free((tProcess*) process->stackTop);
  free(process);
}

void getProcessData(tProcess* process, tProcessData* data) {
  data->name = process->name;
  data->memory = process->stackBase - process->stackTop;
  data->pid = process->pid;
  if (process->status == BLOCKED) {data->status = "Blocked";}
  else {data->status = "Ready";}
  if (process->priority == HIGHP) {data->priority = "High";}
  else if (process->priority == MIDP) {data->priority = "Medium";}
  else {data->priority = "Low";}
}