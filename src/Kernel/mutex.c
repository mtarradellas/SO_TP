#include "include/memoryManager.h"
#include "include/mutex.h"
#include "include/lib.h"
#include "include/scheduler.h"
#include "include/process.h"

int _mutexAcquire(int *mutexValue);
queue_t mutexQueue;
void _interrupt();

mutex_t mutexCreate() {
  mutex_t m = malloc(sizeof(tMutex));
  m->value = 0;
  m->ownerPID = 0;    // why? 0?
  m->lockedQueue = queueCreate(sizeof(tProcess*));
  return m;
}


void mutexDelete(mutex_t mutex) {
  queueFree(mutex->lockedQueue);
  free(mutex);
}

void mutexLock(mutex_t mutex) {
  //printf("locking mutex: %d\n", mutex);
  //printf("mutex value: %d\n", mutex->value);
  tProcess* running = getCurrrentProcess();
  //printf("running process: %s\n", running->name);
  if (!_mutexAcquire(&(mutex->value))) {
    //printf("-libre:\n");
    mutex->ownerPID = running->pid;
  } else {
    //printf("-busy:\n");
    queueOffer(mutex->lockedQueue, &running);
    removeProcess(running);
    running->status = BLOCKED;
    _interrupt();
  }
}

void mutexUnlock(mutex_t mutex) {
  //if (mutex->ownerPID != getCurrrentProcess()->pid) return;
  //printf("unlocking mutex: %d\n", mutex);
  //printf("mutex value: %d\n", mutex->value);
  if (queueSize(mutex->lockedQueue) != 0) {
    //printf("waiting queue != 0\n");
    tProcess* proc = NULL;
    queuePoll(mutex->lockedQueue, &proc);
    mutex->ownerPID = proc->pid;
    proc->status = READY;
    addProcess(proc);
  } else {
    // is this really necessary?
    mutex->ownerPID = 0;
  }
  mutex->value = 0;
  //printf("mutex liberado: %d\n", mutex->value);
}
