#include <stdlib.h>
#include "process.h"
#include "scheduler.h"

#define QUANTUM 10

typedef struct tRange {
	int from;
	int to;
} tRange;

typedef struct tPList {
	tProcess *process;
	tRange *tickRange;
	struct tPList *next;
} tPList;;


static int runTicket(int ticket);
static int inRange(tRange *range, int num);

static tPList *processList;
static tPList *auxList;
static int tickets;
static int winner;
static int quantum = QUANTUM;
static tProcess *running; // FALTA GUARDAR SU DATA EN EL SWITCH


void start(tProcess initProcess) {
	tickets = HIGHP + 1;
	tProcess *idle = newProcess("systemIdle", ??);
	processList = newPList(aux, HIGHP);
	processList->process->code();

	pList.tickets = HIGHP;

}

void addProcess(tProcess proc, int priority){
	tPList *curr = mallocMemory(sizeof(tPlist*));
	curr->tickRange = mallocMemory(sizeof(tRange*));
	curr->process = proc;
	curr->next = processList;
	curr->tickRange->from = tickets;
	curr->tickRange->to = tickets + priority;
	tickets+=priority;
	processList = curr;
}

static void freeNode(tPList curr){
	freeMemory(curr->process);
	freeMemory(curr->tickRange);
	freeMemory(curr->next);
}

static tPList * recRem(tPlist aux,tProcess proc){
	if(aux == NULL)
		return NULL;
		if(aux->process == proc){
			tPList* other = aux->next;
			freeNode(aux);
			return other;
	}
	aux->tickrange->from -= proc->priority;
	aux->tickrange->to -= proc->priority;
	aux->next = recRem(aux->next,proc);
	return aux;
}


void removeProcess(tProcess proc){
	auxList = processList;
	tickets-= proc->priority;
	processList = recRem(auxList, proc);
	updateTickets();
}

void lottery() {
	if (quantum != 0) {
		quantum--;
	}
	else {
		winner = rand() % tickets;
		while(runTicket(winner) != 1) {
			winner = rand() % tickets;
		}
		quantum = QUANTUM;
		(running->code)();
	}
}

static int runTicket(int ticket) {
	auxList = processList;
	while(auxList != NULL) {
		if (inRange(auxList->tickRange, ticket)) {
			// BUSCAR MEMORIA DE AUX
			// GUARDAR MEMORIA DEL RUNNING
			running = auxList->process;
			return 1;
		}
		auxList = auxList->next;
	}
	return 0;
}

static int inRange(tRange *range, int num) {
	return num >= range->from && num < range->to;
}

int rand() {
	return 322323;
}