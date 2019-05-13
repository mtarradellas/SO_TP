#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "./mutex.h"
#include "./queue.h"
typedef struct {
  int value;
  queue_t lockedQueue;
  mutex_t mutex;
} tSemaphore;

typedef tSemaphore* sem_t;

sem_t semCreate(int startValue);
void semDelete(sem_t sem);
// void semSet(sem_t sem, int value);
void semWait(sem_t sem);
void semPost(sem_t sem);

#endif
