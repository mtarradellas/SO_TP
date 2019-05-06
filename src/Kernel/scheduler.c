#include <stddef.h>
#include "process.h"
#include "scheduler.h"
#include "interruptions.h"
#include "lib.h"
#include "timeDriver.h"
#include "memoryManager.h"
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
int _initProcess(uint64_t stackBase, int (*entry)(int, char **), int argc, char **argv);

void addProcess(tProcess *proc, int priority);
static void freeNode(tPList *curr);
static tPList * recRem(tPList *list, tProcess *proc, int *procTickets);
void removeProcess(tProcess *proc);
static int runTicket(int ticket, uint64_t rsp);
static int inRange(tRange *range, int num);

static tPList *processList = NULL;
static tPList *auxList;
static int tickets = 0;
static int winner;
static int quantum = QUANTUM;
static tProcess *running;

int testrand();

void start(int (*entryPoint)(int, char**)) {
	tProcess *shell = newProcess("shell", entryPoint, 0, NULL);
	if (shell == NULL) {
		// throw error
		return;
	}
	addProcess(shell, HIGHP);
	running = shell;
	_runProcess(running->rsp);
}

void addProcess(tProcess *proc, int priority) {
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
	new->tickRange->to = tickets + priority - 1;
	new->priority = priority;
	tickets += priority;
	processList = new;
}

static void freeNode(tPList *node) {
	free(node->process);
	free(node->tickRange);
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////  ///////  ///////////////////////////////////////////////////////
//////// T E S T S ///////////////////////  ///////  ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////           ///////////////////////////////////////////////////////
///////////////////////////////////////  /////////////  ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

void schedTestDinamic() {
	char *str1 = "1 ~ ";
	char* vec1[1];
	vec1[0] = str1;
	char *str2 = "2 ~ ";
	char* vec2[1];
	vec2[0] = str2;
	tProcess *one = newProcess("One", (mainFNC)fncOne, 1, vec1);
	tProcess *two = newProcess("Two", (mainFNC)fncTwo, 1, vec2);

	addProcess(one, 1);
	addProcess(two, 1);

	running = one;
	//removeProcess(two);
	putStr("start:\n");
	_runProcess(running->rsp);
	while(1){

	}
}

void schedTestStatic(uint64_t initAdress) {
	putStr("welcome\n");
	char *str1 = "1 ~ ";
	char *vec1[1];
	vec1[0] = str1;
	uint64_t memAd = initAdress + 1000000;
	tProcess tOne;
	tProcess *one = &tOne;
	one->pid = 1;
	one->entry = (mainFNC)fncOne;
	one->argc = 1;
	one->argv = vec1;
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
	char *str2 = "2 ~ ";
	char *vec2[1];
	vec2[0] = str2;
	tProcess tTwo;
	tProcess *two = &tTwo;
	two->pid = 2;
	two->entry = (mainFNC)fncTwo;
	two->argc = 1;
	two->argv = vec2;
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