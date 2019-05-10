#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

#define READY 0
#define BLOCKED 1

typedef struct tProcess {
  unsigned long int pid;
  char *name;
  int (*entry)(int, char **argv);
  uint64_t stackBase;
  uint64_t stackTop;
  uint64_t rsp;
  int status;
  int argc;
  char **argv;
} tProcess;

struct tProcess *newProcess(char *name, int (*entry)(int, char **), int argc,
                            char **argv);

void initPids();

#endif
