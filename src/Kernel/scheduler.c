#include <stdlib.h>
#include "process.h"
#include "scheduler.h"
#include "interruptions.h"
#include "lib.h"
#include "timeDriver.h"
//////////////////////TESTS
#include "videoDriver.h"
typedef int (*mainFNC)();

#define QUANTUM 0

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

void _cli();
void _sti();
void _interrupt();

void _runProcess(uint64_t rsp); //jumps to rsp stack and continues its program execution
int _initProcess(uint64_t stackBase, int (*entry)(int, char**), int argc, char **argv);

void addProcess(tProcess *proc, int priority);
static void freeNode(tPList *curr);
static tPList * recRem(tPList *list, tProcess *proc, int *procTickets);
void removeProcess(tProcess *proc);
static int runTicket(int ticket, uint64_t rsp);
static int inRange(tRange *range, int num);

static tPList *processList;
static tPList *auxList;
static int tickets;
static int winner;
static int quantum = QUANTUM;
static tProcess *running; // FALTA GUARDAR SU DATA EN EL SWITCH

int testrand();

void start(void *initProcess) {
	addProcess((tProcess *) initProcess, HIGHP);
	// Add system idle
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
	if (processList == NULL) {return; }
	if (quantum != 0) {
		quantum--;
		return;
	}
	else {
		winner = testrand() % tickets;
		while(runTicket(winner, rsp) != 1) {
			winner = testrand() % tickets;
		}
		quantum = QUANTUM;
		_sti();
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
	return (num >= range->from) && (num <= range->to);
}

char* getProcList() {/*
	char* strg = mallocMemory(BLOQUE);
	int j=0;
	auxList = processList;
	while(auxList != NULL){ // considero que el nombre de los procesos tienen un ' /0' al final
		int i=0;
		while (auxList->process->name[i] != '/0'){
			if(j%BLOQUE == 0){
				strg = reallocMemory(strg,sizeof(char*)*(j+BLOQUE));
			}
			strg[j++] = auxList->process->name[i++];
		}
		strg[j] = '\n';
		auxList = auxList->next;
	}
	strg = reallocMemory(strg,sizeof(char*) * j);*/
	return NULL;//strg;

}
//////// T E S T S ////////////////////////////////////

void fncOne() {
	while(1) {
		putStr("1 ");
		for (int i = 0; i < 3000000; i++) {
			;
		}
		_interrupt();
	}
}

void fncTwo() {
	while(1) {
		putStr("2 ");
		for (int i = 0; i < 3000000; i++) {
			;
		}
		_interrupt();
	}
}

void schedTest(uint64_t initAdress) {
	uint64_t memAd = initAdress + 1000000;
	tProcess tOne;
	tProcess *one = &tOne;
	one->pid = 1;
	one->entry = (mainFNC)fncOne;
	one->argc = 0;
	one->argv = NULL;
	one->stackBase = memAd;
	one->stackTop = memAd - 4000;
	one->status = READY;
	one->rsp = _initProcess(one->stackBase, one->entry, one->argc, one->argv);

	tRange tRangeOne;
	tRange *rangeOne = &tRangeOne;
	rangeOne->from = 0;
	rangeOne->to = 0;

	tPList tlistOne;
	tPList *listOne = &tlistOne;
	listOne->process = one;
	listOne->tickRange = rangeOne;
	listOne->priority = 1;
	///////////////////////////////////////////////////////////////////////////////////////
	tProcess tTwo;
	tProcess *two = &tTwo;
	two->pid = 2;
	two->entry = (mainFNC)fncTwo;
	two->argc = 0;
	two->argv = NULL;
	two->stackBase = memAd - 8000 ;
	two->stackTop = memAd - 11999;
	two->status = READY;
	two->rsp = _initProcess(two->stackBase, two->entry, two->argc, two->argv);

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
	running = one; 
	tickets = 2;
	//////////////////////////////////
	_runProcess(running->rsp);
	while(1) {
	}
}
int random = 0;
int testrand() {
	random++;
	return random;
}