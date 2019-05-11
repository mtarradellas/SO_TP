#include "include/memoryManager.h"
#include "include/mutex.h"
#include "include/lib.h"
#include "include/scheduler.h"

void _interrupt();

mutex_t mutexCreate() {
  mutex_t m = malloc(sizeof(tMutex));
  m->value = 0;
  m->ownerPID = -1;
  m->lockedQueue = newQueue();
  return m;
}


void mutexDelete(mutex_t mutex) {
  freeQueue(mutex->lockedQueue);
  free(mutex);
}

void mutexLock(mutex_t mutex) {
  tProcess* running = getCurrrentProcess();
  if (_mutex_acquire(&(mutex->value))) {
    mutex->ownerPID = running->pid;
  } else {
    offer(mutex->lockedQueue, running);
    removeProcess(running);
  }
}

void mutexUnlock(mutex_t mutex) {
  if (mutex->ownerPID != getCurrrentProcess()->pid) return;

  while (mutex->lockedQueue != 0) {
    tProcess* proc = poll(mutex->lockedQueue);
    mutex->ownerPID = proc->pid;
    addProcess(proc);
    return;
  }
  mutex->value = 0;
}

