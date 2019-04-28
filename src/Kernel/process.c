#include "process.h"
#include "stdlib.h"

static long int id = 0;

tProcess * newProcess(char *name, void *code, int priority) {

	tProcess *aux = malloc(sizeof(*aux));
	aux->name = name;
	aux->code = code;
	aux->pid = id;
	aux->priority = priority;
	id++;
	return aux;

}