#ifndef PROCESS_H
#define PROCESS_H

#define READY 0
#define BLOCKED 1

typedef struct tProcess {
	long int pid;
	char *name;
	void *code;
	void *stackBase;
	void *stackTop;
	int status;
	int priority;
} tProcess;

tProcess * newProcess(char *name, void *code, void *stackBase, int priority);

#endif