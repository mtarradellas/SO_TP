#include <stdint.h>
#include "include/SYSCDispatcher.h"
#include "include/keyboardDriver.h"
#include "include/memoryManager.h"
#include "include/mutex.h"
#include "include/semaphore.h"
#include "include/process.h"
#include "include/scheduler.h"
#include "include/timeDriver.h"
#include "include/videoDriver.h"

#include "include/lib.h"

typedef enum {
  READ,
  WRITE,
  WAIT,
  GETTIME,
  GETSCREENSIZE,
  DRAWCIRCLE,
  DRAWRECTANGLE,
  BEEPON,
  BEEPOFF,
  GETCURSOR,
  SETCURSOR,
  MALLOC,
  REALLOC,
  FREE,
  CREATEPROC,
  PRINTNODE,
  KILL,
  PS,
  WAITPID,
  MUTEXOPEN,
  MUTEXCLOSE,
  MUTEXLOCK,
  MUTEXUNLOCK,
  SEMOPEN,
  SEMCLOSE,
  SEMWAIT,
  SEMPOST
} Syscall;

typedef enum { CHARACTER, DRAWCHAR, CLEAR, STRING } Write;

typedef enum { HOUR, MINUTE, SECOND } Time;

void beepon();
void beepoff();

void _cli();
void _sti();

static void _read(char *c);
static void _write(uint64_t mode, uint64_t p1, uint64_t p2, uint64_t p3,
                   uint64_t p4);
static void _getTime(unsigned int *dest, uint64_t time);
static void _wait(int *sec);
static void _getScreenSize(int *x, int *y);
static void _drawCircle(Color *color, int *radius, int *x, int *y);
static void _drawRectangle(Color *color, int *b, int *h, int *x, int *y);
static void _getCursor(int *x, int *y);
static void _setCursor(int *x, int *y);
static void _beepon();
static void _beepoff();
static void _malloc(void **dest, size_t size);
static void _realloc(void *src, size_t size, void **dest);
static void _free(void *src);
static void _printNode(void *src);
static unsigned long int _createProc(char *name, int (*entry)(int, char **),
                                     int argc, char **argv, int priority);
static void _kill(unsigned long int pid);
static void _ps(tProcessData ***psVec, int *size);
static void _waitpid(unsigned long int pid);

static int _mutexOpen(char id[MAX_MUTEX_ID]);
static int _mutexClose(char id[MAX_MUTEX_ID]);
static int _mutexLock(char id[MAX_MUTEX_ID]);
static int _mutexUnlock(char id[MAX_MUTEX_ID]);

static int _semOpen(char id[MAX_SEM_ID], int start);
static int _semClose(char id[MAX_SEM_ID]);
static int _semWait(char id[MAX_SEM_ID]);
static int _semPost(char id[MAX_SEM_ID]);

typedef struct tProcList {
  tProcess *process;
  struct procList *next;
} tProcList;

typedef struct tReadMutex {
  int keys;
  tProcList *readQueue;
} tReadMutex;

tReadMutex readMutex;

typedef uint64_t (*SystemCall)();

SystemCall syscall_array[] = {
    (SystemCall)_read,          (SystemCall)_write,
    (SystemCall)_wait,          (SystemCall)_getTime,
    (SystemCall)_getScreenSize, (SystemCall)_drawCircle,
    (SystemCall)_drawRectangle, (SystemCall)_beepon,
    (SystemCall)_beepoff,       (SystemCall)_getCursor,
    (SystemCall)_setCursor,     (SystemCall)_malloc,
    (SystemCall)_realloc,       (SystemCall)_free,
    (SystemCall)_createProc,    (SystemCall)_printNode,
    (SystemCall)_kill,          (SystemCall)_ps,
    (SystemCall)_waitpid,       (SystemCall)_mutexOpen,
    (SystemCall)_mutexClose,    (SystemCall)_mutexLock,
    (SystemCall)_mutexUnlock,   (SystemCall)_semOpen,
    (SystemCall)_semClose,      (SystemCall)_semWait,
    (SystemCall)_semPost  };
void syscallDispatcher(uint64_t syscall, uint64_t p1, uint64_t p2, uint64_t p3,
                       uint64_t p4, uint64_t p5) {
  syscall_array[syscall](p1, p2, p3, p4, p5);
}

/*void signalAddedKey() {
  readMutex.keys++;

}*/

static void _read(char *c) {
  *c = getKey();
}

static void _write(uint64_t mode, uint64_t p1, uint64_t p2, uint64_t p3,
                   uint64_t p4) {
  switch (mode) {
    case CHARACTER:
      printChar(*((char *)p1), *((Color *)p2));
      break;
    case DRAWCHAR:
      drawChar(*((char *)p1), *((int *)p3), *((int *)p4), *((Color *)p2));
    case CLEAR:
      clear();
      break;
    case STRING:
      putStr((char *)p1);
      break;
  }
}

static void _wait(int *sec) { wait(*sec); }

static void _getTime(unsigned int *t, uint64_t time) {
  switch (time) {
    case HOUR:
      *t = getHour();
      break;
    case MINUTE:
      *t = getMinute();
      break;
    case SECOND:
      *t = getSecond();
      break;
  }
}

static void _getScreenSize(int *x, int *y) { getScreenSize(x, y); }

static void _drawCircle(Color *color, int *radius, int *x, int *y) {
  drawCircle(*color, *radius, *x, *y);
}

static void _drawRectangle(Color *color, int *b, int *h, int *x, int *y) {
  drawRectangle(*color, *b, *h, *x, *y);
}

static void _beepon() { beepon(); }

static void _beepoff() { beepoff(); }

static void _getCursor(int *x, int *y) { getCursor(x, y); }

static void _setCursor(int *x, int *y) { setCursor(*x, *y); }

static void _malloc(void **dest, size_t size) { *dest = malloc(size); }

static void _realloc(void *src, size_t size, void **dest) {
  *dest = realloc(src, size);
}

static void _free(void *src) { free(src); }

static unsigned long int _createProc(char *name, int (*entry)(int, char **),
                                     int argc, char **argv, int priority) {
  tProcess *newP = newProcess(name, entry, argc, argv, priority);
  initStack(newP);
  addProcess(newP);
  return newP->pid;
}

static void _printNode(void *src) { printNode(src); }

static void _kill(unsigned long int pid) { killProc(pid); }

static void _ps(tProcessData ***psVec, int *size) { 
  _cli();
  ps(psVec, size); 
  _sti();
}

static void _waitpid(unsigned long int pid) {
  _sti();
  while (getProcess(pid) != NULL) {
    ;
  }
}

static int mutexCmp(void *a, void *b) {
  char *left = (*((MutexData **)a))->id;
  char *right = (*((MutexData **)b))->id;

  return strcmp(left, right);
}

static int _mutexOpen(char id[MAX_MUTEX_ID]) {
  if (mutexQueue == NULL) {
    mutexQueue = queueCreate(sizeof(MutexData*));
  }
  MutexData* data;
  queueResetIter(mutexQueue);
  while (queueGetNext(mutexQueue, &data) == 0) {
    if (strcmp(id, data->id) == 0) {
      return 1;
    }
  }
  data = malloc(sizeof(MutexData));
  memcpy(data->id, id, strlen(id) + 1);
  data->mutex = mutexCreate();
  queueOffer(mutexQueue, &data);
  return 0;
}

static int _mutexClose(char id[MAX_MUTEX_ID]) {
  if (mutexQueue == NULL) return 1;
  MutexData* data;
  queueResetIter(mutexQueue);
  while (queueGetNext(mutexQueue, &data) == 0) {
    if (strcmp(id, data->id) == 0) {
      mutexDelete(data->mutex);
      queueRemove(mutexQueue, &mutexCmp, &data);
      free(data);
      return 1;
    }
  }
  return 2;
}
static int _mutexLock(char id[MAX_MUTEX_ID]) {
  if (mutexQueue == NULL) return 1;
  MutexData* data;
  queueResetIter(mutexQueue);
  while (queueGetNext(mutexQueue, &data) == 0) {
    if (strcmp(id, data->id) == 0) {
      mutexLock(data->mutex);
      return 0;
    }
  }
  return 2;
}
static int _mutexUnlock(char id[MAX_MUTEX_ID]) {
  if (mutexQueue == NULL) return 1;
  MutexData* data;
  queueResetIter(mutexQueue);
  while (queueGetNext(mutexQueue, &data) == 0) {
    if (strcmp(id, data->id) == 0) {
      mutexUnlock(data->mutex);
      return 0;
    }
  }
  return 2;
}

static int semCmp(void *a, void *b) {
  char *left = (*((SemData **)a))->id;
  char *right = (*((SemData **)b))->id;

  return strcmp(left, right);
}

static int _semOpen(char id[MAX_MUTEX_ID], int start) {
  if (semQueue == NULL) {
    semQueue = queueCreate(sizeof(SemData*));
  }
  SemData* data;
  queueResetIter(semQueue);
  while (queueGetNext(semQueue, &data) == 0) {
    if (strcmp(id, data->id) == 0) {
      return 1;
    }
  }
  data = malloc(sizeof(SemData));
  memcpy(data->id, id, strlen(id) + 1);
  data->sem = semCreate(start);
  queueOffer(semQueue, &data);
  return 0;
}

static int _semClose(char id[MAX_MUTEX_ID]) {
  if (semQueue == NULL) return 1;
  SemData* data;
  queueResetIter(semQueue);
  while (queueGetNext(semQueue, &data) == 0) {
    if (strcmp(id, data->id) == 0) {
      semDelete(data->sem);
      queueRemove(semQueue, &semCmp, &data);
      free(data);
      return 1;
    }
  }
  return 2;
}
static int _semWait(char id[MAX_MUTEX_ID]) {
  if (semQueue == NULL) return 1;
  SemData* data;
  queueResetIter(semQueue);
  while (queueGetNext(semQueue, &data) == 0) {
    if (strcmp(id, data->id) == 0) {
      semWait(data->sem);
      return 0;
    }
  }
  return 2;
}
static int _semPost(char id[MAX_MUTEX_ID]) {
  if (semQueue == NULL) return 1;
  SemData* data;
  queueResetIter(semQueue);
  while (queueGetNext(semQueue, &data) == 0) {
    if (strcmp(id, data->id) == 0) {
      semPost(data->sem);
      return 0;
    }
  }
  return 2;
}