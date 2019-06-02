#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

#define HIGHP 250
#define MIDP 150
#define LOWP 50
#define IDLE 1

#define READY 0
#define BLOCKED 1

#define MAX_FD 30
#define STD_IN 0
#define STD_OUT 1


typedef struct tProcess {
  unsigned long int pid;
  unsigned long int parent;
  int fileDescriptors[MAX_FD];
  int maxFD;
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

void initializeProcesses();
void freeProcess(tProcess* process);
void getProcessData(tProcess* process, tProcessData* data);
void ps(tProcessData*** psVec, int* size);
tProcess* getProcess(unsigned long int pid);
int addFileDescriptor(tProcess* process, int fileDescriptor);
void dup(tProcess* process, int fd, int pos);

#endif
