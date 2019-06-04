#include "include/semaphore.h"
#include "include/lib.h"
#include "include/memoryManager.h"
#include "include/scheduler.h"

queue_t semQueue;
void _interrupt();

sem_t semCreate(int startValue) {
  sem_t sem = malloc(sizeof(tSemaphore));
  if (sem == NULL) return NULL;
  sem->value = startValue;
  sem->lockedQueue = queueCreate(sizeof(tProcess*));
  sem->mutex = mutexCreate();
  return sem;
}

int semGetValue(sem_t sem) { return sem->value; }

void semDelete(sem_t sem) {
  mutexDelete(sem->mutex);
  queueFree(sem->lockedQueue);
  free(sem);
}

void semWait(sem_t sem) {

  if (sem == NULL) {
    return;
  }
  mutexLock(sem->mutex);
  tProcess* running = getCurrentProcess();
  if (sem->value == 0) {
    queueOffer(sem->lockedQueue, &running);
    mutexUnlock(sem->mutex);
    removeProcess(running);
    _interrupt();
  } else {
    sem->value--;
    mutexUnlock(sem->mutex);
  }
}

void semPost(sem_t sem) {
  if (sem == NULL) return;
  mutexLock(sem->mutex);
  if (queueSize(sem->lockedQueue) != 0) {
    tProcess* proc;
    queuePoll(sem->lockedQueue, &proc);
    addProcess(proc);
  } else {
    sem->value++;
  }
  mutexUnlock(sem->mutex);
}
