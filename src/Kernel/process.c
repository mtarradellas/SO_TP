#include "include/process.h"
#include <stddef.h>
#include "include/memoryManager.h"

#include "include/lib.h"
#include "include/videoDriver.h"

#define DEFAULT_PROC_MEM 4096  // 8k

typedef struct tPList {
  tProcess* process;
  struct tPList* next;
} tPList;

void _cli();
void _sti();
static void addP(tProcess* process);
static tPList* removeP(tPList* node, tProcess* process);

static long int id;
tPList* list;

tProcess* newProcess(char* name, int (*entry)(int, char**), int argc,
                     char** argv, int priority) {
  tProcess* newP = malloc(sizeof(tProcess));
  if (newP == NULL) {
    // throw error
    return NULL;
  }
  newP->pid = id++;
  newP->name = malloc(sizeof(strlen(name) + 1));
  memcpy(newP->name, name, strlen(name) + 1);
  newP->entry = entry;
  newP->argc = argc;
  newP->argv = argv;
  newP->stackBase = (uint64_t)malloc(DEFAULT_PROC_MEM) + DEFAULT_PROC_MEM - 1;
  if ((void*)newP->stackBase == NULL) {
    // throw error
    return NULL;
  }
  newP->stackTop = newP->stackBase - DEFAULT_PROC_MEM + 1;
  newP->rsp = newP->stackBase;
  newP->priority = priority;
  newP->status = READY;
  addP(newP);
  return newP;
}

static void addP(tProcess* process) {
  tPList* aux = malloc(sizeof(tPList));
  aux->process = process;
  aux->next = list;
  list = aux;
}

static tPList* removeP(tPList* node, tProcess* process) {
  if (node == NULL) return NULL;
  if (node->process == process) {
    tPList* aux = node->next;
    free(node);
    return aux;
  }
  node->next = removeP(node->next, process);
  return node;
}

void initializeProcesses() {
  id = 0;
  list = NULL;
}

void freeProcess(tProcess* process) {
  if (process == NULL) return;
  _cli();
  list = removeP(list, process);
  _sti();
  free((tProcess*)process->stackTop);
  free(process);
}

void getProcessData(tProcess* process, tProcessData* data) {
  data->name = malloc(strlen(process->name) + 1);
  memcpy(data->name, process->name, strlen(process->name) + 1);
  data->memory = process->stackBase - process->stackTop;
  data->pid = process->pid;
  if (process->status == BLOCKED) {
    data->status = "Blocked";
  } else {
    data->status = "Ready  ";
  }
  if (process->priority == HIGHP) {
    data->priority = "High   ";
  } else if (process->priority == MIDP) {
    data->priority = "Medium ";
  } else if (process->priority == LOWP) {
    data->priority = "Low   ";
  } else {
    data->priority = "Minimum";
  }
}

void ps(tProcessData*** psVec, int* size) {
  tPList* auxList = list;
  tProcessData** auxVec = NULL;
  int s = 0;
  while (auxList != NULL) {
    auxVec = realloc(auxVec, (s + 1) * sizeof(tProcessData*));
    auxVec[s] = malloc(sizeof(tProcessData));
    getProcessData(auxList->process, auxVec[s]);
    s++;
    auxList = auxList->next;
  }
  (*psVec) = auxVec;
  (*size) = s;
}

tProcess* getProcess(unsigned long int pid) {
  tPList* aux = list;
  while (aux != NULL) {
    if (aux->process->pid == pid) {
      return aux->process;
    }
    aux = aux->next;
  }
  return NULL;
}
