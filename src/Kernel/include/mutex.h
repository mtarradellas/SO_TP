#ifndef MUTEX_H
#define MUTEX_H
#include <stddef.h>
#include "./queue.h"
#include "./process.h"

#define MAX_MUTEX_ID 30

typedef struct tMutex {
  int value;
  unsigned long ownerPID;
  queue_t lockedQueue;
} tMutex;

typedef tMutex* mutex_t;

typedef struct MutexData {
  char id[MAX_MUTEX_ID];
  mutex_t mutex;
}MutexData;

extern queue_t mutexQueue;


mutex_t mutexCreate();
void mutexDelete(mutex_t mutex);
void mutexLock(mutex_t mutex);
void mutexUnlock(mutex_t mutex);
int _mutex_acquire(int * lockValue);

#endif
