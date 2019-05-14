#ifndef QUEUE_ADT_H
#define QUEUE_ADT_H
#include <stddef.h>
typedef struct QueueStruct* queue_t;
// creates a Queue, and returns a queue_t which is a pointer to a Queue struct
// if there is not enough memory to create the queue, returns NULL
queue_t queueCreate(size_t bytes);

// adds elem to the end of the queue.
// returns 1 if the queue was not itialized
// returns 2 if there's not enough memory to add the element
// returns 0 if there were no errors
int queueOffer(queue_t queue, void* elem);

// removes the first element from the queue and leaves it in ret
// returns 1 if the queue was not initialized
// returns 2 if the queue is empty
// returns 0 if there were no errors
int queuePoll(queue_t queue, void* ret);

// iterates over the queue, leaving the current element in ret
// returns 1 if the queue was not initialized
// returns 2 if there are no next elements
// returns 0 if there were no erros
int queueGetNext(queue_t queue, void* ret);

// resets the iterator to the first element of the queue
// returns 1 if the queue was not initialized
// returns 0 if there were no errors
int queueResetIter(queue_t queue);

// looks for an element in the queue that matches value using the cmp function,
// leaves it in ret
// returns 1 if the queue was not initialized
// returns 2 if the queue is empty
// returns 3 inf the element was not found
// returns 0 if there were no errors
int queueFind(queue_t queue, int (*cmp)(void*, void*), void* value, void* ret);

// looks for an element in the queue that matches value using the cmp function,
// removes it from the queue
// returns 1 if the queue was not initialized
// returns 2 if the queue is empty
// returns 3 inf the element was not found
// returns 0 if there were no errors
int queueRemove(queue_t queue, int (*cmp)(void*, void*), void* elem);

// frees the queue's allocated resources
void queueFree(queue_t queue);

// returns the amount of elements in the queue
int queueSize(queue_t queue);

#endif
