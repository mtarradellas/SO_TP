#ifndef PIPE_H
#define PIPE_H

#include "semaphore.h"

typedef struct tPipe {
  int id;
  char* base;
  long readPos;
  long writePos;
  int dataAmount;
  sem_t dataSem;
  mutex_t dataMutex;

}tPipe;

typedef tPipe* pipe_t;

void initializePipes();
int pipe(int fileDescriptors[2]);
int writeToPipe(int id, char* buffer, int bytes);
int readFromPipe(int id, char* buffer, int bytes);

#endif