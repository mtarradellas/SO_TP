#include <stdlib.h>
#include "process.h"
#include "scheduler.h"
#include "interruptions.h"
#include "lib.h"
#include "timeDriver.h"
//////////////////////TESTS
#include "videoDriver.h"

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

void _runProcess(uint64_t rsp); //jumps to rsp stack and continues its program execution

void addProcess(tProcess *proc, int priority);
static void freeNode(tPList *curr);
static tPList * recRem(tPList *list, tProcess *proc, int *procTickets);
void removeProcess(tProcess *proc);
static int runTicket(int ticket);
static int inRange(tRange *range, int num);

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
	/*
	srand(getSecond());
	tPList *curr = mallocMemory(sizeof(tPlist*));
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

void lottery(uint64_t rsp) {
	if (processList == NULL) return;
	if (quantum != 0) {
		quantum--;
		return;
	}
	else {
		winner = rand() % tickets;
		while(runTicket(winner, uint64_t rsp) != 1) {
			winner = rand() % tickets;
		}
		quantum = QUANTUM;
		_runProcess(running->rsp);
	}
}

static int runTicket(int ticket, uint64_t rsp) {
	auxList = processList;
	while(auxList != NULL) {
		if (inRange(auxList->tickRange, ticket)) {
			running->rsp = rsp;
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
//////// T E S T S ////////////////////////////////////

void fncOne() {
	while(1) {
		putStr(" in One ");
		wait(2);
	}
}

void fncTwo() {
	while(1) {
		putStr(" in Two ");
		wait(5);
	}
}

void schedTest(uint8_t endOfKernel) {
	_cli();
	tProcess tOne;
	tProcess *one = &tOne;
	one->pid = 1;
	one->code = fncOne;
	one->stackBase = endOfKernel + 4000;
	one->stackTop = NULL;
	one->status = READY;

	_initProcess(one->stackBase, one->entry, one->argc, one->argv);

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
	two->stackBase = endOfKernel + 8000 ;
	two->stackTop = NULL;
	two->status = READY;

	_initProcess(two->stackBase, two->entry, two->argc, two->argv);

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
		wait(1);
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