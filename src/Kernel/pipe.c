#include "include/pipe.h"
#include "include/memoryManager.h"
#include "include/process.h"
#include "include/queue.h"

#define PIPE_MEM

static queue_t pipeQueue;
static int pipeID;

int pipe(int fileDescriptors[2]) {
  if (pipeQueue == NULL) pipeQueue = queueCreate(sizeof(tPipe));
  pipe_t newPipe = malloc(sizeof(tPipe));
  newPipe->id = pipeID++;
  newPipe->buffer = malloc(PIPE_MEM);
  newPipe->readPTR = newPipe->writePTR = newPipe->buffer;
  tProcess* process = getCurrentProcess();
  fileDescriptors[0] = addFileDescriptor(process, newPipe->id);
  fileDescriptors[1] = addFileDescriptor(process, newPipe->id);
  queueOffer(pipeQueue, newPipe);
  return 0;
}
