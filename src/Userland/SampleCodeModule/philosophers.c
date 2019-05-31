#include "include/philosophers.h"
#include "include/memoryModule.h"
#include "include/mutexModule.h"
#include "include/processModule.h"
#include "include/queue.h"
#include "include/semModule.h"
#include "include/stdlib.h"
#include "include/timeModule.h"
#include "include/videoModule.h"

typedef struct philosopher {
  int id;
  int pid;
} philosopher_t;

static void printMenu();
static void philosopherCreate();
static int cmp(void* a, void* b);
static int philosopher(int argc, char** argv);
static void philosopherKill();
static void philosopherSelfdestruct();
static void philosophersKillAll();
static void printStatus();

int philosophersQty, philosophersToDie, id, indexToDie;
philosopher_t philosophers[MAX_PHILOSOPHERS] = {{0}};
queue_t thinkingList, eatingList;

void philosophersRun() {
  printMenu();
  mutexOpen("philosophers");
  mutexOpen("eating");
  philosophersQty = 0;
  semOpen("forks", 0);
  semOpen("inTable", 0);
  philosophersToDie = 0;
  id = 0;
  indexToDie = 0;

  thinkingList = queueCreate(sizeof(int));
  eatingList = queueCreate(sizeof(int));

  int c;
  while ((c = getChar()) != QUIT) {
    switch (c) {
      case INC:
        philosopherCreate();
        break;
      case DEC:
        philosopherKill();
        break;
      default:
        break;
    }
  }

  philosophersKillAll();
  mutexClose("philosophers");
  mutexClose("eating");
  semClose("forks");
  semClose("inTable");
  printf("End of program\n");
}

static void printMenu() {
  printf("\nThe table can have a maximum of %d philosophers\n",
         MAX_PHILOSOPHERS);
  printf("Insert:\n");
  printf("    %c to create a philosopher\n", INC);
  printf("    %c to kill a philosopher\n", DEC);
  printf("    %c to quit\n\n", QUIT);
}

static void philosopherCreate() {
  mutexLock("philosophers");
  if (philosophersQty >= MAX_PHILOSOPHERS) {
    printf("The max capacity for philosophers has been reached.\n\n\n");
    mutexUnlock("philosophers");
    return;
  }
  semPost("forks");
  int index = id++;
  philosopher_t phi = {};
  phi.pid =
      createProcess("philosopher", (mainf)philosopher, index, NULL, HIGHP);
  phi.id = index;
  indexToDie = id - 1;
  philosophers[philosophersQty++] = phi;
  if (philosophersQty > 1) semPost("inTable");
  mutexUnlock("philosophers");

  printf("\nphilosopher %d has been created :)\n\n", index);

  mutexLock("eating");
  queueOffer(thinkingList, &index);
  mutexUnlock("eating");
}
static int cmp(void* a, void* b) {
  int left = *((int*)a);
  int right = *((int*)b);
  return right - left;
}
static int philosopher(int argc, char** argv) {
  int i = argc;

  while (1) {
    if (philosophersToDie > 0 && i == indexToDie) {
      philosopherSelfdestruct();
    }

    semWait("inTable");
    semWait("forks");
    semWait("forks");

    mutexLock("eating");
    queueRemove(thinkingList, cmp, &i);
    queueOffer(eatingList, &i);
    printStatus();
    mutexUnlock("eating");

    wait(20);
    semPost("forks");
    semPost("forks");
    semPost("inTable");

    mutexLock("eating");
    queueRemove(eatingList, cmp, &i);
    queueOffer(thinkingList, &i);
    mutexUnlock("eating");
  }
  return 0;
}

static void philosopherKill() {
  mutexLock("philosophers");
  if (philosophersQty == 0) {
    printf("All philosophers are already dead...\n\n");
    mutexUnlock("philosophers");
    return;
  }
  philosophersToDie++;
  mutexUnlock("philosophers");
}

static void philosopherSelfdestruct() {
  mutexLock("philosophers");
  philosophersQty--;
  philosophersToDie--;
  indexToDie = (philosophers[philosophersQty - 1]).id;
  int phiToDie = (philosophers[philosophersQty]).pid;

  mutexUnlock("philosophers");
  semWait("inTable");
  semWait("forks");
  printf("\nphilosopher %d has died :(\n\n", (philosophers[philosophersQty]).id);
  mutexLock("eating");
  queueRemove(thinkingList, cmp, &(philosophers[philosophersQty]).id);
  mutexUnlock("eating");
  kill(phiToDie);
}

static void philosophersKillAll() {
  mutexLock("philosophers");
  for (int i = 0; i < philosophersQty; i++) {
    kill((philosophers[i]).pid);
  }

  queueFree(eatingList);
  queueFree(thinkingList);

  mutexUnlock("philosophers");
}
static void printList(queue_t queue) {
  queueResetIter(queue);
  int next;
  while (queueGetNext(queue, &next) == 0) {
    printf("%d-", next);
  }
  return;
}
static void printStatus() {
  int spacesE = queueSize(eatingList);

  printf("Eating: ");
  printList(eatingList);

  for (int i = 0; i < (3 * MAX_PHILOSOPHERS / 2) - spacesE; i++) {
    printf(" ");
  }

  printf("Thinking: ");
  printList(thinkingList);

  printf("\n");
}
