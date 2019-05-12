#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

#define HIGHP 250
#define MIDP 150
#define LOWP 10

#define READY 0
#define BLOCKED 1

typedef struct tProcess {
  unsigned long int pid;
  char *name;
  int (*entry)(int, char **argv);
  uint64_t stackBase;
  uint64_t stackTop;
  uint64_t rsp;
  int priority;
  int status;
  int argc;
  char **argv;
} tProcess;

typedef struct tProcessData {
  unsigned long int pid;
  char* name;
  char* status;
  int memory;
  char* priority;
} tProcessData;

struct tProcess *newProcess(char *name, int (*entry)(int, char **), int argc,
                            char **argv, int priority);

void initPids();
void freeProcess(tProcess* process);
void getProcessData(tProcess* process, tProcessData* data);
void ps(tProcessData*** psVec, int* size);

#endif
