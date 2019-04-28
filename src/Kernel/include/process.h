#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

typedef struct tProcess {
	long int pid;
	char *name;
	void (*code)();
	void *stackBase;
	void *stackTop;
	int status;
} tProcess;

struct tProcess * newProcess(char *name, void *code);
void runProcess(struct tProcess *process);

#endif