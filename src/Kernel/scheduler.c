#include <stddef.h>
#include "process.h"
#include "scheduler.h"
#include "interruptions.h"
#include "lib.h"
#include "timeDriver.h"
#include "memoryManager.h"
//////////////////////TESTS
#include "videoDriver.h"
#include "SYSCDispatcher.h"

typedef int (*entryFnc)();

#define QUANTUM 0

typedef struct tRange {
	int from;
	int to;
} tRange;

typedef struct tPList {
	struct tProcess *process;
	struct tRange *tickRange;
	struct tPList *next;
} tPList;

void _cli();
void _sti();
void _interrupt();

void _runProcess(uint64_t rsp); //jumps to rsp stack and continues its program execution
uint64_t _initStack(uint64_t stackBase, int (*entry)(int, char **), int argc, char **argv, uint64_t stackRet);

static void freeNode(tPList *curr);
static tPList * recRem(tPList *list, tProcess *proc, int *procTickets);
static int runTicket(int ticket, uint64_t rsp);
static int inRange(tRange *range, int num);
static void endProcess();
void run(int (*entry)(int, char**), int argc, char** argv);
static tProcess* getProcess(unsigned long int pid);
int idle(int argc, char** argv);

static tPList *processList;
static tPList *auxList;
static int tickets;
static int winner;
static int quantum;
static tProcess *running;

int testrand();

int idle(int argc, char** argv) {
	while(1){
		putStr("i");
	}
	return 0;
}

void start(int (*entryPoint)(int, char**)) {
	processList = NULL;
	tickets = 0;
	quantum = QUANTUM;
	initializeMM();
	initPids();
	tProcess* shell = newProcess("shell", entryPoint, 0, NULL, HIGHP);
	if (shell == NULL) {
		// throw error
		return;
	}
	initStack(shell);
	addProcess(shell);
	//tProcess* sysIdle = newProcess("system_idle", idle, 1, NULL); // Le estoy dando 4k de mem pero a quien le importa
	//addProcess(sysIdle, 1);
	running = shell;
	_runProcess(running->rsp);
}

void run(int (*entry)(int, char**), int argc, char** argv) {
	entry(argc, argv);
	_cli();
	endProcess();
}

void endProcess() {
	removeProcess(running);
	freeProcess(running);
	running = NULL;
	_interrupt();
}

void addProcess(tProcess *proc) {
	tPList *new = malloc(sizeof(*new));
	if (new == NULL) {
		// throw error
		putStr("list node null");
	}
	new->tickRange = malloc(sizeof(*(new->tickRange)));
	if (new->tickRange == NULL) {
		// throw error
		putStr("tickRange null");
	}
	new->process = proc;
	new->next = processList;
	new->tickRange->from = tickets;
	new->tickRange->to = tickets + proc->priority - 1;
	tickets += proc->priority;
	processList = new;
}

static void freeNode(tPList *node) {
	free(node->tickRange);
	free(node);
}

static tPList * recRem(tPList *list, tProcess *proc, int *procTickets) {
	if(list == NULL)
		return NULL;
	if(list->process == proc) {
		*procTickets = proc->priority;
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

void removeProcess(tProcess* process) {
	int procTickets = 0;
	processList = recRem(processList, process, &procTickets);
}

void killProc(unsigned long int pid) {
	_cli();
	tProcess* p = getProcess(pid);
	removeProcess(p);
	freeProcess(p);
	_sti();
	_interrupt();
}

void lottery(uint64_t rsp) {
	if (processList == NULL) {return; }
	if (quantum != 0) {
		quantum--;
		return;
	}
	else {
		winner = rand() % tickets;
		while(runTicket(winner, rsp) != 1) {
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
			if (running != NULL) running->rsp = rsp;
			running = auxList->process;
			return 1;
		}
		auxList = auxList->next;
	}
	return 0;
}

tProcess* getCurrrentProcess() {
	return running;
}

static tProcess* getProcess(unsigned long int pid) {
	auxList = processList;
	while(auxList != NULL && auxList->process->pid != pid) {
		auxList = auxList->next;
	}
	if (auxList == NULL) {
		return NULL;
	}
	return auxList->process;
}

void initStack(tProcess* proc) {
	proc->rsp = _initStack(proc->stackBase, proc->entry, proc->argc, proc->argv, (uint64_t)run);
}

static int inRange(tRange *range, int num) {
	return (num >= range->from) && (num <= range->to);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////  ///////  ///////////////////////////////////////////////////////
//////// T E S T S ///////////////////////  ///////  ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////           ///////////////////////////////////////////////////////
///////////////////////////////////////  /////////////  ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void fncOne(int argc, char *argv[]) {
	while(1) {
		putStr(argv[0]);
		for (int i = 0; i < 300000000; i++) {
			;
		}
		//_interrupt();
	}
}

void fncTwo(int argc, char *argv[]) {
	while(1) {
		putStr(argv[0]);
		for (int i = 0; i < 300000000; i++) {
			;
		}
		//_interrupt();
	}
}

void test1() {
  while(1) {
    putStr(" 1 ");
    wait(10);
  }
  return;
}

void test2() {
  while(1) {
    putStr(" 2 ");
    wait(10);
  }
}

// con mallocs
void schedTestDinamic() {

	processList = NULL;
	tickets = 0;
	quantum = QUANTUM;
	initializeMM();
	initPids();

	tProcess* one = newProcess("one", test1, 0, NULL);
  addProcess(one, 10);

  tProcess* two = newProcess("two", test2, 0, NULL);
  addProcess(two, 10);

  running = two;
	putStr("start:\n");
	_sti();
	_runProcess(running->rsp);
	printProcList();
	while(1){

	}
}
*/
void printProcList() {
	auxList = processList;
	while(auxList != NULL) {
		putStr(auxList->process->name);
		auxList = auxList->next;
	}
}
/*
// sin mallocs
void schedTestStatic(uint64_t initAdress) {
	putStr("welcome\n");
	char *str1 = "1 ~ ";
	char *vec1[1];
	vec1[0] = str1;
	uint64_t memAd = initAdress + 1000000;
	tProcess tOne;
	tProcess *one = &tOne;
	one->pid = 1;
	one->entry = (entryFnc)fncOne;
	one->argc = 1;
	one->argv = vec1;
	one->stackBase = memAd;
	one->stackTop = memAd - 4000;
	one->status = READY;
	//one->rsp = _initProcess(one->stackBase, one->entry, one->argc, one->argv);

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
	char *str2 = "2 ~ ";
	char *vec2[1];
	vec2[0] = str2;
	tProcess tTwo;
	tProcess *two = &tTwo;
	two->pid = 2;
	two->entry = (entryFnc)fncTwo;
	two->argc = 1;
	two->argv = vec2;
	two->stackBase = memAd - 8000 ;
	two->stackTop = memAd - 11999;
	two->status = READY;
	//two->rsp = _initProcess(two->stackBase, two->entry, two->argc, two->argv);

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
	putStr("run:\n");
	_runProcess(running->rsp);
	while(1) {
	}
}
int random = 0;
int testrand() {
	random++;
	return random;
}
*/