#include "include/pipe.h"
#include "include/memoryManager.h"
#include "include/process.h"
#include "include/queue.h"
#include "include/scheduler.h"
#include "include/lib.h"
#include "include/semaphore.h"

#define PIPE_MEM 4096 // 4k

static queue_t pipeQueue;
static int pipeID;

static pipe_t getPipe(int id);
static void freePipe(pipe_t pipe);
static int cmp(void* a, void* b);

void initializePipes() {
  pipeID = 2;
  //free pipe queue
}

int pipe(int fileDescriptors[2]) {
  if (pipeQueue == NULL) {
    pipeQueue = queueCreate(sizeof(sem_t));
  }
  pipe_t newPipe = malloc(sizeof(tPipe));
  newPipe->id = pipeID++;
  newPipe->base = malloc(PIPE_MEM);
  newPipe->readPos = newPipe->writePos = 0;
  newPipe->dataSem = semCreate(0);
  newPipe->dataMutex = mutexCreate();
  newPipe->dataAmount = 0;
  newPipe->users = 2;
  tProcess* process = getCurrentProcess();
  fileDescriptors[0] = addFileDescriptor(process, newPipe->id);
  fileDescriptors[1] = addFileDescriptor(process, newPipe->id);
  queueOffer(pipeQueue, &newPipe);
  return 0;
}

int readFromPipe(int id, char* buffer, int bytes) {
  pipe_t pipe = getPipe(id);
  semWait(pipe->dataSem);
  mutexLock(pipe->dataMutex);
  if (pipe == NULL) {
    //error*/
    mutexUnlock(pipe->dataMutex);
    return 0;
  }
  int i;
  for (i = 0; i < bytes && pipe->dataAmount > 0; i++) {
    pipe->readPos = pipe->readPos % PIPE_MEM;
    buffer[i] = pipe->base[pipe->readPos++];
    pipe->dataAmount--;
  }
  mutexUnlock(pipe->dataMutex);
  return i;
}

int writeToPipe(int id, char *buffer, int bytes) {
  pipe_t pipe = getPipe(id);
  mutexLock(pipe->dataMutex);
  if (pipe == NULL) {
    //error
    mutexUnlock(pipe->dataMutex);
    return 0;
  }
  int i;
  for (i = 0; i < bytes; i++) {
    pipe->writePos = pipe->writePos % PIPE_MEM;
    pipe->base[pipe->writePos++] = buffer[i];
    pipe->dataAmount++;
  }
  if (bytes > 0 && semGetValue(pipe->dataSem)==0) semPost(pipe->dataSem);
  mutexUnlock(pipe->dataMutex);
  return i;
}

static pipe_t getPipe(int id) {
  pipe_t pipe;
  queueResetIter(pipeQueue);
  while (queueGetNext(pipeQueue, &pipe) == 0) {
    if (id == pipe->id) {
      return pipe;
    }
  }
  return NULL;
}

void closeFD(tProcess* process, int fd) {
  int pipeID = process->fileDescriptors[fd];
  if (pipeID == -1) return;
  process->fileDescriptors[fd] = -1;
  pipe_t pipe = getPipe(pipeID);
  pipe->users--;
  if (fd == process->maxFD) setMaxFD(process);
  if (pipe->users == 0) freePipe(pipe);
}

static void freePipe(pipe_t pipe) {
  queueRemove(pipeQueue, cmp, &pipe);
  free(pipe->base);
  semDelete(pipe->dataSem);
  mutexDelete(pipe->dataMutex);
  free(pipe);
}

static int cmp(void* a, void*b) {
  pipe_t p1 = *((pipe_t*)a);
  pipe_t p2 = *((pipe_t*)b);
  return p1->id - p2->id;
}
