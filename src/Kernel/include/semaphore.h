#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "./mutex.h"
#include "./queue.h"

#define MAX_SEM_ID 30

typedef struct {
  int value;
  queue_t lockedQueue;
  mutex_t mutex;
} tSemaphore;

typedef tSemaphore* sem_t;

typedef struct SemData {
  char id[MAX_SEM_ID];
  sem_t sem;
}SemData;

extern queue_t semQueue;

sem_t semCreate(int startValue);
void semDelete(sem_t sem);
// void semSet(sem_t sem, int value);
void semWait(sem_t sem);
void semPost(sem_t sem);

#endif
