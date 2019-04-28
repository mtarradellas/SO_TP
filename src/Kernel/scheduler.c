#include <stdlib.h>
#include "process.h"
#include "scheduler.h"
#include "interruptions.h"
//////////////////////TESTS
#include "videoDriver.h"
#include "lib.h"
#include "timeDriver.h"

#define QUANTUM 1

typedef struct tRange {
	int from;
	int to;
} tRange;

typedef struct tPList {
	struct tProcess *process;
	struct tRange *tickRange;
	int priority;
	struct tPList *next;
} tPList;

void addProcess(tProcess *proc, int priority);
static void freeNode(tPList *curr);
static tPList * recRem(tPList *list, tProcess *proc, int *procTickets);
void removeProcess(tProcess *proc);
static int runTicket(int ticket);
static int inRange(tRange *range, int num);
int rand();


static int runTicket(int ticket);
static int inRange(tRange *range, int num);

static tPList *processList;
static tPList *auxList;
static int tickets;
static int winner;
static int quantum = QUANTUM;
static tProcess *running; // FALTA GUARDAR SU DATA EN EL SWITCH

static int random = 0;


void start(tProcess *initProcess) {
	addProcess(initProcess, HIGHP);
	runProcess(initProcess);
}

void addProcess(tProcess *proc, int priority) {
	/*tPList *curr = mallocMemory(sizeof(tPlist*));
	curr->tickRange = mallocMemory(sizeof(tRange*));
	curr->process = proc;
	curr->next = processList;
	curr->tickRange->from = tickets;
	curr->tickRange->to = tickets + priority - 1;
	curr->priority = priority;
	tickets += priority;
	processList = curr;*/
}

static void freeNode(tPList *curr) {
	//freeMemory(curr->process);
	//freeMemory(curr->tickRange);
}

static tPList * recRem(tPList *list, tProcess *proc, int *procTickets) {
	if(list == NULL)
		return NULL;
	if(list->process == proc) {
		*procTickets = list->priority;
		tickets -= *procTickets;
		tPList* aux = list->next;
		freeNode(list);
		return aux;
	}
	list->next = recRem(list->next, proc, procTickets);
	list->tickRange->from -= *procTickets;
	list->tickRange->to -= *procTickets;
	return list;
}

void removeProcess(tProcess *proc) {
	int procTickets = 0;
	processList = recRem(processList, proc, &procTickets);
}

void lottery() {
	if (processList == NULL) return;
	if (quantum != 0) {
		quantum--;
	}
	else {
		winner = rand() % tickets;
		while(runTicket(winner) != 1) {
			winner = rand() % tickets;
		}
		quantum = QUANTUM;
		runProcess(running);
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
	return num >= range->from && num <= range->to;
}

int rand() {
	random++;
	return random;
}

void fncOne() {
	putStr(" in One ");
}

void fncTwo() {
	putStr(" in Two ");
}

void schedTest(uint8_t endOfKernel) {
	_cli();
	tProcess tOne;
	tProcess *one = &tOne;
	one->pid = 1;
	one->code = fncOne;
	one->stackBase = NULL;
	one->stackTop = NULL;
	one->status = READY;

	tRange tRangeOne;
	tRange *rangeOne = &tRangeOne;
	rangeOne->from = 0;
	rangeOne->to = 0;

	tPList tlistOne;
	tPList *listOne = &tlistOne;
	listOne->process = one;
	listOne->tickRange = rangeOne;
	listOne->priority = 1;
	//////////////////////////////////
	tProcess tTwo;
	tProcess *two = &tTwo;
	two->pid = 2;
	two->code = fncTwo;
	two->stackBase = NULL;
	two->stackTop = NULL;
	two->status = READY;

	tRange tRangeTwo;
	tRange *rangeTwo = &tRangeTwo;
	rangeTwo->from = 1;
	rangeTwo->to = 1;

	tPList tlistTwo;
	tPList *listTwo = &tlistTwo;
	listTwo->process = two;
	listTwo->tickRange = rangeTwo;
	listTwo->priority = 1;	
	//////////////////////////////////
	listOne->next = NULL;
	listTwo->next = listOne;
	processList = listTwo;
	tickets = 2;
	//////////////////////////////////
	_sti();
	while(1) {
		//putStr(" ~ ");
	}
	/*
	int i = 0;
	while(i < 30) {
		lottery();
		wait(1);
		i++;
	}
	putStr(" salio ");
	removeProcess(two);
	putStr(" borro ");
	while(1) {
		lottery();
		wait(1);
	}
	while(1) {
		lottery();
		wait(5);
	}*/
}