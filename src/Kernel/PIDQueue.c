#include "include/PIDQueue.h"
#include "include/memoryManager.h"

PIDQueue newQueue() {
  PIDQueue queue = malloc(sizeof(*queue));
  queue->first = NULL;
  queue->last = NULL;
  queue->size = 0;
  return queue;
}

void offer(PIDQueue queue, unsigned long elem) {
  if (queue == NULL) {
    return;
  }
  tNode *aux = malloc(sizeof(tNode));
  // what if malloc fails?

  aux->elem = elem;
  aux->next = NULL;
  if (queue->first == NULL) {
    queue->first = queue->last = aux;
  } else {
    queue->last->next = aux;
    queue->last = aux;
  }
  queue->size++;
  return;
}

unsigned long poll(PIDQueue queue) {
  unsigned long value;
  tNode *temp = queue->first;
  if (temp != NULL) {
    value = temp->elem;
    queue->first = queue->first->next;
    free(temp);
  }

  queue->size--;
  return value;
}

int getSize(PIDQueue queue) { return queue->size; }

void freeQueue(PIDQueue queue) {
  if (queue == NULL) {
    return;
  }
  tNode *node = queue->first;
  while (node != NULL) {
    tNode *aux = node;
    node = node->next;
    free(aux);
  }
  free(queue);
}