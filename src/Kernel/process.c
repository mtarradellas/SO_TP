#include <stdlib.h>
#include "include/process.h"

#define READY 0
#define BLOCKED 1
#define PROCMEM 4096  // 4k

static long int id = 0;

tProcess *newProcess(char *name, int (*entry)(int, char *), int argc,
                     char *argv) {
  /*tProcess *aux = mallocMemory(sizeof(*aux));
  aux->name = name;
  aux->entry = entry;
  aux->pid = id++;
  aux->stackBase = mallocMemory(PROCMEM);
  aux->stackTop = aux->stackBase + PROCMEM;
  aux->status = READY;
*/
  return NULL;  // aux;
}
