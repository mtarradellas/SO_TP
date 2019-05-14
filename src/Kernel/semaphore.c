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

void semDelete(sem_t sem) {
  mutexDelete(sem->mutex);
  queueFree(sem->lockedQueue);
  free(sem);
}

void semWait(sem_t sem) {
  if (sem == NULL) return;
  mutexLock(sem->mutex);
  tProcess* running = getCurrrentProcess();
  if (sem->value == 0) {
    queueOffer(sem->lockedQueue, &running);
    removeProcess(running);
    mutexUnlock(sem->mutex);
  } else {
    sem->value--;
    mutexUnlock(sem->mutex);
  }
}

void semPost(sem_t sem) {
  if (sem == NULL) return;

  mutexLock(sem->mutex);
  if (queueSize(sem->lockedQueue) != 0) {
      tProcess* proc = NULL;
      queuePoll(sem->lockedQueue, &proc);
      addProcess(proc);
  }
  sem->value++;
  mutexUnlock(sem->mutex);
}
