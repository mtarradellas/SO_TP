// #include "include/mutex.h"
// #include "include/scheduler.h"
// #include "include/lib.h"
// mutex_t mutexCreate() {
//   mutex_t m = malloc(sizeof(tMutex));
//   m->value = 0;
//   m->ownerPID = -1;
//   m->lockedQueue = NULL;
//   return m;
// }

// static int acquire(int* value) {
//   int ret = *value;
//   *value = 1;
//   return ret;
// }

// void mutexDelete(mutex_t mutex) { free(mutex); }

// void mutexLock(mutex_t mutex) {
//   if (!acquire(&(mutex->value))) {
//     mutex->ownerPID = getPID();
//   } else {
//     unsigned long pid = getPID();
//     mutex->lockedQueue = offer(mutex->lockedQueue, pid);
//     blockProcess(pid);
//     _force_scheduler();
//   }
// }

// void mutexUnlock(mutex_t mutex) {
//   if (mutex->ownerPID != getPID()) return;

//   while (mutex->lockedQueue != 0) {
//     unsigned long pid = poll(mutex->lockedQueue);
//     mutex->ownerPID = pid;
//     unblockProcess(pid);
//     return;
//   }
//   mutex->value = 0;
// }
