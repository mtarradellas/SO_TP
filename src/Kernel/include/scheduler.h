#ifndef SCHEDULER_H
#define SCHEDULER_H

#define HIGHP 100
#define MIDP 60
#define LOWP 40

void start(struct tProcess *initProcess);
void lottery();

#endif