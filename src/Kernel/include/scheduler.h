#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include "process.h"

#define HIGHP 100
#define MIDP 60
#define LOWP 40

#define READY 0
#define BLOCKED 1

void start(int (*entryPoint)(int, char**));
void lottery(uint64_t rsp);
void addProcess(tProcess *proc);
void removeProcess(tProcess *proc);
tProcess* getRunning();
void initStack(tProcess* proc);
void killProc(unsigned long int pid);

char *getProcList();
void schedTestDinamic();
void schedTestStatic(uint64_t initStack);

#endif