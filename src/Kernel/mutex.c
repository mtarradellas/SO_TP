#include "include/memoryManager.h"
#include "include/mutex.h"
#include "include/lib.h"
#include "include/scheduler.h"

int _mutexAcquire(int *mutexValue);
queue_t mutexQueue;
void _interrupt();

mutex_t mutexCreate() {
  mutex_t m = malloc(sizeof(tMutex));
  m->value = 0;
  m->ownerPID = 1;    // why? 0?
  m->lockedQueue = queueCreate(sizeof(tProcess*));
  return m;
}


void mutexDelete(mutex_t mutex) {
  queueFree(mutex->lockedQueue);
  free(mutex);
}

void mutexLock(mutex_t mutex) {
  tProcess* running = getCurrrentProcess();
  if (!_mutexAcquire(&(mutex->value))) {
    mutex->ownerPID = running->pid;
  } else {
    queueOffer(mutex->lockedQueue, &running);
    removeProcess(running);
    _interrupt();
  }
}

void mutexUnlock(mutex_t mutex) {
  if (mutex->ownerPID != getCurrrentProcess()->pid) return;

  if (queueSize(mutex->lockedQueue) != 0) {
    tProcess* proc = NULL;
    queuePoll(mutex->lockedQueue, &proc);
    mutex->ownerPID = proc->pid;
    addProcess(proc);
  } else {
    // is this really necessary?
    mutex->ownerPID = 0;
  }

  mutex->value = 0;
}
