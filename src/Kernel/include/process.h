#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

typedef struct tProcess {
	long int pid;
	char *name;
	void (*code)();
	void *stackBase;
	void *stackTop;
	uint64_t rsp;
	int status;
} tProcess;

struct tProcess * newProcess(char *name, void *code);

#endif