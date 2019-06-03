#include <stddef.h>
#include "include/nice.h"
#include "include/process.h"
#include "include/scheduler.h"

void nice(unsigned long int pid, int priority) {
  tProcess* aux;
  aux = getProcess(pid);
  if (aux != NULL) {
    removeProcess(aux);
    aux->priority = priority;
    addProcess(aux);
  }
}
