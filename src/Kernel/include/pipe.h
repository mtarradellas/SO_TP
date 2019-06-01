#ifndef PIPE_H
#define PIPE_H

typedef struct tPipe {
  int id;
  char* base;
  long readPos;
  long writePos;
}tPipe;

typedef tPipe* pipe_t;

void initializePipes();
int pipe(int fileDescriptors[2]);
int writeToPipe(int id, char* buff, int size);

#endif