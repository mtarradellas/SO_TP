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
char *getProcList();
void addProcess(tProcess *proc, int priority);

void schedTestDinamic();
void schedTestStatic(uint64_t initStack);

#endif