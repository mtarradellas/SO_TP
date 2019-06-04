#include "./include/queue.h"
#include "./include/lib.h"
#include "./memoryManager.h"
// TESTS
#include "./include/pipe.h"

typedef struct Node {
  void* data;
  struct Node* next;
} Node;

typedef struct QueueStruct {
  size_t bytes;
  int size;
  Node* first;
  Node* last;
  Node* current;
} QueueStruct;

queue_t queueCreate(size_t bytes) {
  queue_t queue = malloc(sizeof(QueueStruct));
  if (queue == NULL) return queue;
  queue->bytes = bytes;
  queue->size = 0;
  queue->first = NULL;
  queue->last = NULL;
  queue->current = NULL;
  return queue;
}

int queueOffer(queue_t queue, void* elem) {
  if (queue == NULL) return 1;
  Node* newNode = malloc(sizeof(Node));
  void* newData = malloc(sizeof(queue->bytes));
  if (newNode == NULL || newData == NULL) return 2;
  memcpy(newData, elem, queue->bytes);
  newNode->next = NULL;
  newNode->data = newData;
  if (queue->first == NULL) {
    queue->first = queue->last = queue->current = newNode;
  } else {
    queue->last->next = newNode;
    queue->last = newNode;
  }

  queue->size++;
  return 0;
}

int queuePoll(queue_t queue, void* ret) {
  if (queue == NULL) return 1;
  if (queue->first == NULL) return 2;
  memcpy(ret, queue->first->data, queue->bytes);
  Node* aux = queue->first->next;
  if (queue->current == queue->first) {
    queue->current = queue->first->next;
  }
  free(queue->first->data);
  free(queue->first);
  queue->size--;
  queue->first = aux;
  return 0;
}

void queueFree(queue_t queue) {
  Node* aux;
  while (queue->first != NULL) {
    aux = queue->first->next;
    free(queue->first->data);
    free(queue->first);
    queue->first = aux;
  }

  free(queue);
}
int queueGetNext(queue_t queue, void* ret) {
  if (queue == NULL) return 1;
  if (queue->current == NULL) return 2;
  memcpy(ret, queue->current->data, queue->bytes);

  queue->current = queue->current->next;
  return 0;
}
int queueFind(queue_t queue, int (*cmp)(void*, void*), void* value, void* ret) {
  if (queue == NULL) return 1;
  if (queue->first == NULL) return 2;

  if ((*cmp)(queue->first->data, value) == 0) {
    memcpy(ret, queue->first->data, queue->bytes);
    return 0;
  }

  Node* aux = queue->first;
  while (aux->next != NULL) {
    if ((*cmp)(aux->next->data, value) == 0) {
      memcpy(ret, aux->next->data, queue->bytes);
      return 0;
    }
    aux = aux->next;
  }
  return 3;
}
int queueRemove(queue_t queue, int (*cmp)(void*, void*), void* elem) {
  if (queue == NULL) return 1;
  if (queue->first == NULL) return 2;

  Node* next = queue->first->next;
  if ((*cmp)(queue->first->data, elem) == 0) {
    free(queue->first->data);
    free(queue->first);
    queue->first = next;
    queue->size--;
    return 0;
  }

  Node* aux = queue->first;
  while (aux->next != NULL) {
    if ((*cmp)(aux->next->data, elem) == 0) {
      Node* aux2 = aux->next->next;
      free(aux->next->data);
      free(aux->next);
      aux->next = aux2;
      queue->size--;
      return 0;
    }
    aux = aux->next;
  }
  return 3;
}

int queueResetIter(queue_t queue) {
  if (queue == NULL) return 1;
  queue->current = queue->first;
  return 0;
}

int queueSize(queue_t queue) {
  if (queue == NULL) return 0;
  return queue->size;
}
