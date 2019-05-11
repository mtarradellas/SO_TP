#include "include/procQueue.h"
#include "include/memoryManager.h"

procQueue newQueue() {
  procQueue queue = malloc(sizeof(*queue));
  queue->first = NULL;
  queue->last = NULL;
  queue->size = 0;
  return queue;
}

void offer(procQueue queue, tProcess* elem) {
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

tProcess* poll(procQueue queue) {
  tProcess* value;
  tNode *temp = queue->first;
  if (temp != NULL) {
    value = temp->elem;
    queue->first = queue->first->next;
    free(temp);
  }

  queue->size--;
  return value;
}

int getSize(procQueue queue) { return queue->size; }

void freeQueue(procQueue queue) {
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
