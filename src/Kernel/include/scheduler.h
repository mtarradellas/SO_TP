#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include "process.h"

#define READY 0
#define BLOCKED 1

void start(int (*entryPoint)(int, char**));
void lottery(uint64_t rsp);
void addProcess(tProcess *proc);
void removeProcess(tProcess* process);
tProcess* getCurrrentProcess();
void initStack(tProcess* proc);
void killProc(unsigned long int pid);

void printProcList();
void schedTestDinamic();
void schedTestStatic(uint64_t initStack);

#endif