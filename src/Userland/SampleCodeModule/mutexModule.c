#include "include/mutexModule.h"
#include "include/SYSCall.h"
#include "include/stdlib.h"

void mutexOpen(char id[MAX_MUTEX_ID]) {
  systemCall((uint64_t)MUTEXOPEN, (uint64_t)id, 0, 0, 0, 0);
}
void mutexClose(char id[MAX_MUTEX_ID]) {
  systemCall((uint64_t)MUTEXCLOSE, (uint64_t)id, 0, 0, 0, 0);
}
void mutexLock(char id[MAX_MUTEX_ID]) {
  systemCall((uint64_t)MUTEXLOCK, (uint64_t)id, 0, 0, 0, 0);
}
void mutexUnlock(char id[MAX_MUTEX_ID]) {
  systemCall((uint64_t)MUTEXUNLOCK, (uint64_t)id, 0, 0, 0, 0);
}
