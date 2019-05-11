#ifndef PROC_QUEUE_H
#define PROC_QUEUE_H

#include "./process.h"

typedef struct tNode {
  struct tNode* next;
  tProcess* elem;
} tNode;

typedef struct {
  tNode* last;
  tNode* first;
  int size;
} queueStruct;

typedef queueStruct* procQueue;

procQueue newQueue();

void freeQueue(procQueue q);

void offer(procQueue q, tProcess* elem);

tProcess* poll(procQueue q);

int getSize(procQueue q);

int getSize(procQueue q);

#endif
