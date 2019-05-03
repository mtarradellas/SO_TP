#ifndef SCHEDULER_H
#define SCHEDULER_H

#define HIGHP 100
#define MIDP 60
#define LOWP 40

#define READY 0
#define BLOCKED 1

void start(struct tProcess *initProcess);
void lottery();

#endif