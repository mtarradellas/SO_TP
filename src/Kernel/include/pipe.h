#ifndef PIPE_H
#define PIPE_H

typedef struct tPipe {
  int id;
  char* buffer;
  char* readPTR;
  char* writePTR;
}tPipe;

typedef tPipe* pipe_t;

int pipe(int fileDescriptors[2]);

#endif