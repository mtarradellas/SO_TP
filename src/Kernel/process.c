#include "process.h"
#include "stdlib.h"

#define READY 0
#define BLOCKED 1
#define PROCMEM 4096 // 4k

static long int id = 0;

tProcess * newProcess(char *name, void *code) {

	/*tProcess *aux = mallocMemory(sizeof(*aux));
	aux->name = name;
	aux->code = code;
	aux->pid = id++;
	aux->stackBase = mallocMemory(PROCMEM);
	aux->stackTop = aux->stackBase + PROCMEM;
	aux->status = READY;
*/
	return NULL;//aux;
}

void runProcess(tProcess *process) {
	(process->code)();
}