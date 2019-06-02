#include <stddef.h>
#include "process.h"
#include "scheduler.h"
#include "interruptions.h"
#include "lib.h"
#include "timeDriver.h"
#include "memoryManager.h"
#include "mutex.h"
#include "semaphore.h"
//////////////////////TESTS
#include "videoDriver.h"
#include "SYSCDispatcher.h"
#include "EXCDispatcher.h"
#include "include/pipe.h"
#include "include/lib.h"

typedef int (*entryFnc)();

#define QUANTUM 0

sem_t readSem;
extern queue_t semQueue;
extern queue_t mutexQueue;

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
void _exceptionStackOverflowHandler();
void _signalEOI();

void _runProcess(uint64_t rsp); //jumps to rsp stack and continues its program execution
uint64_t _initStack(uint64_t stackBase, int (*entry)(int, char **), int argc, char **argv, uint64_t stackRet);

static void freeNode(tPList *curr);
static tPList * recRem(tPList *list, tProcess *proc, int *procTickets);
static int runTicket(int ticket, uint64_t rsp);
static int inRange(tRange *range, int num);
static void endProcess();
void run(int (*entry)(int, char**), int argc, char** argv);
static tProcess* getSchedProcess(unsigned long int pid);
static void idle();

static tPList *processList;
static tPList *auxList;
static int tickets;
static int winner;
static int quantum;
static tProcess *running = NULL;

int testrand();

void start(int (*entryPoint)(int, char**)) {
	processList = NULL;
	tickets = 0;
	quantum = QUANTUM;
	running = NULL;
	initializeMM();
	initializeProcesses();
	initializePipes();
	mutexQueue = NULL;
	semQueue = NULL;
	readSem = semCreate(0);
	tProcess* sys_idle = newProcess("sysIdle", (entryFnc)idle, 0, NULL, IDLE);
	tProcess* shell = newProcess("shell", entryPoint, 0, NULL, HIGHP);
	if (shell == NULL) {
		// throw error
		return;
	}
	if (sys_idle == NULL) {
		// throw error
		return;
	}
	initStack(shell);
	initStack(sys_idle);
	addProcess(shell);
	addProcess(sys_idle);
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
	}
	new->tickRange = malloc(sizeof(*(new->tickRange)));
	if (new->tickRange == NULL) {
		// throw error
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
	_cli();
	int procTickets = 0;
	processList = recRem(processList, process, &procTickets);
	if (processList == NULL) {
		running = NULL;
	}
	_sti();
}

void killProc(unsigned long int pid) {
	int r = 0;
	if (pid == running->pid) r = 1; 
	tProcess* p = getSchedProcess(pid);
	removeProcess(p);
	freeProcess(p);
	if (r == 1) _interrupt();
}

void lottery(uint64_t rsp) {
	if (running != NULL && rsp < running->stackTop) {
		// stack overflow
		_exceptionStackOverflowHandler();
	}
	if (processList == NULL) {
		return;
	}
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

static void idle(void) {
	_sti();
	_signalEOI();
	while(1){
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

tProcess* getCurrentProcess() {
	return running;
}

static tProcess* getSchedProcess(unsigned long int pid) {
	_cli();
	auxList = processList;
	while(auxList != NULL && auxList->process->pid != pid) {
		auxList = auxList->next;
	}
	if (auxList == NULL) {
		_sti();
		return NULL;
	}
	_sti();
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

void printProcList() {
	auxList = processList;
	while(auxList != NULL) {
		putStr(auxList->process->name);
		auxList = auxList->next;
	}
}

void pipeTest();
void sonTest();

void startTest(int (*entryPoint)(int, char**)) {
	processList = NULL;
	tickets = 0;
	quantum = QUANTUM;
	running = NULL;
	initializeMM();
	initializeProcesses();
	initializePipes();
	mutexQueue = NULL;
	semQueue = NULL;
	readSem = semCreate(0);
	////////////////////////
	tProcess* sys_idle = newProcess("sysIdle", (entryFnc)idle, 0, NULL, IDLE);
	initStack(sys_idle);
	addProcess(sys_idle);
  ////////////////////////
	tProcess* pipeTestProc = newProcess("pipeTest", (entryFnc)pipeTest, 0, NULL, HIGHP);
	initStack(pipeTestProc);
	addProcess(pipeTestProc);
	////////////////////////
	running = pipeTestProc;
	_runProcess(running->rsp);
}

void pipeTest() {
	printf("\n~~~~T E S T 1~~~~\n");
	printf("Starting Pipe Test\n");
	int fd[2];
	if (pipe(fd) < 0 ) {
		printf("Pipe creation error\n");
	}
	printf("Pipe created!\n");
	printf("File descriptors: %d - %d\n", fd[0], fd[1]);
	printf("Writing to pipe\n");
	write(fd[1], "~lenia~", 8);
	printf("Writing done!\n");

	char buffer[100] = {0};
	printf("Reading pipe\n");
	int readB = read(fd[0], buffer, 10);
	printf("Reading done!\n");
	printf("S T R I N G: %s.\ntotal bytes read: %d\n", buffer, readB);
	///////////////////////////////////////////////////////////////////
	wait(10);
  printf("\n\n~~~~T E S T 2~~~~\n");
  
	tProcess* son = newProcess("son", (entryFnc)sonTest, 0, NULL, HIGHP);
	initStack(son);
	dup(son, fd[1], 0);
	dup(son, fd[0], 1);
	addProcess(son);
	wait(5);

	printf("(F) writing to pipe\n");
	write(fd[1], "Lenia", 6);
	wait(10);

	printf("(F) reading from pipe\n");
	char buff[50] = {0};
	read(fd[0], buff, 20);
	printf("(F) string: %s.\n", buff);
	return;
}

void sonTest() {
	char buff[50] = {0};
	printf("(S) reading from std_in (now pipe)\n");
	read(0, buff, 10);
	printf("(S) string %s.\n", buff);

	printf("(S) writing to std_out (now pipe)\n");
	write(1, "franco", 7);
	return;
}