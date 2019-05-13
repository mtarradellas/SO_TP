#ifndef QUEUE_ADT_H
#define QUEUE_ADT_H
#include <stddef.h>
typedef struct QueueStruct* queue_t;

queue_t queueCreate(size_t bytes);
int queueOffer(queue_t queue, void* elem);
int queuePoll(queue_t queue, void* ret);
int queueGetNext(queue_t queue, void* ret);
int queueResetIter(queue_t queue);
int queueFind(queue_t queue, int (*cmp)(void*, void*), void* value, void* ret);
int queueRemove(queue_t queue, int (*cmp)(void*, void*),  void* elem);
void queueFree(queue_t queue);
int queueSize(queue_t queue);

#endif
