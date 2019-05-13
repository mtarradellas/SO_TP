#ifndef MUTEX_H
#define MUTEX_H
#include <stddef.h>
#include "./queue.h"
#include "./process.h"
typedef struct {
  int value;
  unsigned long ownerPID;
  queue_t lockedQueue;
} tMutex;

typedef tMutex* mutex_t;
mutex_t mutexCreate();
void mutexDelete(mutex_t mutex);
void mutexLock(mutex_t mutex);
void mutexUnlock(mutex_t mutex);
int _mutex_acquire(int * lockValue);

#endif
