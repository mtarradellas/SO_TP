#include "include/pipe.h"
#include "include/memoryManager.h"
#include "include/process.h"
#include "include/queue.h"
#include "include/scheduler.h"
#include "include/lib.h"

#define PIPE_MEM 4096 // 4k

static queue_t pipeQueue;
static int pipeID;

static void copyToPipe(char* base, long writePos, char* buffer, int size);
static pipe_t getPipe(int id);

void initializePipes() {
  pipeID = 0;
  //free pipe queue
}

int pipe(int fileDescriptors[2]) {
  if (pipeQueue == NULL) {
    printf("  creating pipe queue\n");
    pipeQueue = queueCreate(sizeof(tPipe));
  }
  pipe_t newPipe = malloc(sizeof(tPipe));
  printf("  creating pipe, ID: %d\n", pipeID);
  newPipe->id = pipeID++;
  newPipe->base = malloc(PIPE_MEM);
  newPipe->readPos = newPipe->writePos = 0;
  tProcess* process = getCurrentProcess();
  fileDescriptors[0] = addFileDescriptor(process, newPipe->id);
  fileDescriptors[1] = addFileDescriptor(process, newPipe->id);
  printf("  adding pipe %d to queue\n", newPipe->id);
  queueOffer(pipeQueue, newPipe);
  return newPipe->id;
}

int writeToPipe(int id, char *buff, int size) {
  pipe_t pipe = getPipe(id);
  if (pipe == NULL) {
    //error
    return 1;
  }
  copyToPipe(pipe->base, pipe->writePos, buff, size);
  return 0;
}

static pipe_t getPipe(int id) {
  pipe_t pipe;
  queueResetIter(pipeQueue);
  printf("  searching for pipe..\n");
  while (queueGetNext(pipeQueue, &pipe) == 0) {
    printf("  is this pipe?\n");
    if (id == pipe->id) {
      printf("  pipe found!\n");
      return pipe;
    }
  }
  printf("  pipe not found\n");
  return NULL;
}

static void copyToPipe(char* base, long writePos, char* buffer, int size) {
  for (int i = 0; i < size; i++) {
    printf("    cpy %c\n", buffer[i]);
    writePos = writePos % PIPE_MEM;
    base[writePos++] = buffer[i];
  }
}