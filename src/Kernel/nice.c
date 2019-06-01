#include <stddef.h>
#include "include/scheduler.h"
#include "include/nice.h"
#include "include/process.h"


void nice(int priority , unsigned long int PID){
  tProcess* aux;
  
  aux = getProcess(PID);
  if(aux != NULL){
    removeProcess(aux);
    aux->priority = priority;
    addProcess(aux);
  }
}