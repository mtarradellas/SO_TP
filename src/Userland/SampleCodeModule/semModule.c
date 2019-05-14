#include "include/semModule.h"
#include "include/SYSCall.h"
//test
#include "stdlib.h"

void semOpen(char id[MAX_SEM_ID], int start) {
  systemCall((uint64_t)SEMOPEN, (uint64_t)id, (uint64_t)&start, 0, 0, 0);
}
void semClose(char id[MAX_SEM_ID]) {
  systemCall((uint64_t)SEMCLOSE, (uint64_t)id, 0, 0, 0, 0);
}
void semWait(char id[MAX_SEM_ID]) {
  systemCall((uint64_t)SEMWAIT, (uint64_t)id, 0, 0, 0, 0);
}
void semPost(char id[MAX_SEM_ID]) {
  systemCall((uint64_t)SEMPOST, (uint64_t)id, 0, 0, 0, 0);
}
