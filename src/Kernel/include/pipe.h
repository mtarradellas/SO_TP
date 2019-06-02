#ifndef PIPE_H
#define PIPE_H

#include "semaphore.h"
#include "process.h"

typedef struct tPipe {
  int id;
  char* base;
  long readPos;
  long writePos;
  int dataAmount;
  sem_t dataSem;
  mutex_t dataMutex;
  int users;
}tPipe;

typedef tPipe* pipe_t;

void initializePipes();
int pipe(int fileDescriptors[2]);
int writeToPipe(int id, char* buffer, int bytes);
int readFromPipe(int id, char* buffer, int bytes);
void closeFD(tProcess* process, int fd);

#endif