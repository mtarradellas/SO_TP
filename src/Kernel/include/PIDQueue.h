#ifndef PIDQUEUE_H
#define PIDQUEUE_H

typedef struct tNode {
  struct tNode* next;
  unsigned long elem;
} tNode;

typedef struct {
  tNode* last;
  tNode* first;
  int size;
} queueStruct;

typedef queueStruct* PIDQueue;

PIDQueue newQueue();

void freeQueue(PIDQueue q);

void offer(PIDQueue q, unsigned long elem);

unsigned long poll(PIDQueue q);

int getSize(PIDQueue q);

int getSize(PIDQueue q);

#endif
